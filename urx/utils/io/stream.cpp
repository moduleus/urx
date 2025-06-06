#include "stream.h"

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/utils/common.h>
#include <urx/utils/export.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer_impl.h>
#include <urx/utils/io/writer_options.h>
#include <urx/utils/raw_data_helper.h>

namespace urx::utils::io {

GroupDataStream::GroupDataStream(std::vector<urx::GroupData>* group_data, H5::Group& h5_group,
                                 urx::utils::io::WriterDataset* writer)
    : _group_data(group_data),
      _group_data_idx(group_data->size() - 1),
      _h5_group_data(h5_group),
      _writer(writer),
      _h5_sequence_timestamps(_h5_group_data.openDataSet("sequence_timestamps")),
      _h5_event_timestamps(_h5_group_data.openGroup("event_timestamps")) {
  if (_h5_group_data.nameExists("raw_data")) {
    _h5_raw_data = _h5_group_data.openDataSet("raw_data");
  }
}

void GroupDataStream::append(const std::shared_ptr<RawData>& raw_data, double sequence_timestamp,
                             const std::vector<double>& event_timestamp) {
  urx::GroupData& group_data = _group_data->at(_group_data_idx);
  group_data.event_timestamps.emplace_back(event_timestamp);

  _extendVector(_h5_group_data, _h5_sequence_timestamps, group_data.sequence_timestamps,
                sequence_timestamp);

  const std::string idx_string =
      common::formatIndexWithLeadingZeros(group_data.event_timestamps.size() - 1, ITER_LENGTH);

  _writer->serializeHdf5(idx_string, event_timestamp, _h5_event_timestamps);

  if (_h5_raw_data.getId() == H5I_INVALID_HID) {
    _writer->serializeHdf5("raw_data", raw_data, _h5_group_data);

    if (_h5_group_data.nameExists("raw_data")) {
      _h5_raw_data = _h5_group_data.openDataSet("raw_data");
    }
  } else {
    _extend(_h5_group_data, _h5_raw_data, raw_data);
  }
}

URX_UTILS_EXPORT urx::GroupData& GroupDataStream::getGroupData() {
  return (*_group_data)[_group_data_idx];
}

template <typename T>
void GroupDataStream::_extendVector(H5::Group& group, H5::DataSet& dataset, std::vector<T>& vector,
                                    const T& value) {
  vector.emplace_back(value);

  const H5::DSetCreatPropList plist = dataset.getCreatePlist();
  const H5D_layout_t layout = plist.getLayout();
  const H5::DataSpace old_space = dataset.getSpace();
  const hsize_t old_size = dataset.getSpace().getSimpleExtentNpoints();
  const H5::PredType& mem_type = *getStdToHdf5().at(nameTypeid<T>());

  if (layout == H5D_CHUNKED) {
    const hsize_t new_size = old_size + 1;
    dataset.extend(&new_size);
    const hsize_t dims[1] = {1};
    old_space.selectHyperslab(H5S_SELECT_SET, dims, &old_size);
    const H5::DataSpace memspace(1, dims);
    dataset.write(&value, mem_type, memspace, old_space);
  } else {
    std::vector<T> data(static_cast<size_t>(old_size));
    dataset.read(data.data(), mem_type);
    data.push_back(value);
    const hsize_t dims[1] = {data.size()};
    const H5::DataSpace new_space(1, dims);
    const std::string dataset_name = dataset.getObjName();
    group.unlink(dataset_name);
    dataset = group.createDataSet(dataset_name, mem_type, new_space);
    dataset.write(data.data(), mem_type);
  }
}

void GroupDataStream::_extend(H5::Group& group, H5::DataSet& dataset,
                              const std::shared_ptr<RawData>& value) {
  if (value->getSize() == 0) {
    return;
  }

  const H5::DataSpace dataspace = dataset.getSpace();
  const H5::DataType datatype = dataset.getDataType();
  const H5::DSetCreatPropList plist = dataset.getCreatePlist();
  const H5D_layout_t layout = plist.getLayout();

  const int ndims = dataspace.getSimpleExtentNdims();
  if (ndims != 2) {
    throw std::runtime_error("Invalid dimension of raw_data");
  }
  hsize_t dimension[2];
  dataspace.getSimpleExtentDims(dimension);

  hsize_t new_dims[2] = {dimension[0] + value->getSize(), dimension[1]};
  if (layout == H5D_CHUNKED) {
    dataset.extend(new_dims);

    const H5::DataSpace filespace = dataset.getSpace();
    const hsize_t start[2] = {dimension[0], 0};
    const hsize_t count[2] = {value->getSize(), dimension[1]};
    filespace.selectHyperslab(H5S_SELECT_SET, count, start);

    const H5::DataSpace memspace(2, count);
    dataset.write(value->getBuffer(), datatype, memspace, filespace);
  } else {
    const std::shared_ptr<urx::RawData> field = urx::utils::rawDataFactory<urx::RawDataNoInit>(
        urx::utils::io::enums::h5PredTypeToDataType(datatype),
        dimension[1] == 1 ? SamplingType::RF : SamplingType::IQ,
        static_cast<size_t>(dimension[0]) + value->getSize());

    if (field->getDataType() != value->getDataType()) {
      throw std::runtime_error("Datatype of RawData to append (" +
                               urx::utils::io::enums::enumToString(value->getDataType()) +
                               ") must have the same data type than the stored RawData (" +
                               urx::utils::io::enums::enumToString(field->getDataType()) + ").");
    }
    if (field->getSamplingType() != value->getSamplingType()) {
      throw std::runtime_error("Sampling type of RawData to append (" +
                               urx::utils::io::enums::enumToString(value->getSamplingType()) +
                               ") must have the same sampling type than the stored RawData (" +
                               urx::utils::io::enums::enumToString(field->getSamplingType()) +
                               ").");
    }

    if (dimension[0] != 0) {
      dataset.read(field->getBuffer(), datatype);
    }

    const size_t unitary_size = urx::utils::group_helper::sizeofDataType(value->getDataType()) *
                                (value->getSamplingType() == SamplingType::RF ? 1 : 2);
    memcpy(static_cast<char*>(field->getBuffer()) + unitary_size * dimension[0], value->getBuffer(),
           unitary_size * value->getSize());

    group.unlink("raw_data");

    _writer->serializeHdf5("raw_data", field, group);
  }
}

Stream::Stream(const std::string& filename, std::shared_ptr<Dataset> dataset)
    : _dataset(std::move(dataset)),
      _file(filename.data(), std::filesystem::exists(filename) ? H5F_ACC_RDWR : H5F_ACC_EXCL) {
  _reader.init(*_dataset);
  _writer.init(*_dataset);
}

std::shared_ptr<Dataset> Stream::getDataset() const { return _dataset; }

void Stream::saveToFile() { _writer.write(_file, *_dataset); }
void Stream::loadFromFile() { _reader.read(_file, *_dataset); }

RawDataLoadPolicy Stream::getRawDataLoadPolicy() const {
  return _reader.getOptions().getRawDataLoadPolicy();
}
void Stream::setRawDataLoadPolicy(RawDataLoadPolicy value) {
  _reader.getOptions().setRawDataLoadPolicy(value);
}

bool Stream::getChunkGroupData() const { return _writer.getOptions().getChunkGroupData(); }
void Stream::setChunkGroupData(bool value) { _writer.getOptions().setChunkGroupData(value); }

GroupDataStream Stream::createGroupData(const std::shared_ptr<Group>& group, DoubleNan timestamp) {
  urx::GroupData group_data;
  group_data.group = group;
  group_data.raw_data =
      urx::utils::rawDataFactory<RawDataNoInit>(group->data_type, group->sampling_type, 0);
  group_data.group_timestamp = timestamp;

  const H5::Group h5_acquisition = _file.openGroup("dataset").openGroup("acquisition");

  H5::Group h5_groups_data;
  if (!h5_acquisition.nameExists("groups_data")) {
    h5_groups_data = h5_acquisition.createGroup("groups_data");
  } else {
    h5_groups_data = h5_acquisition.openGroup("groups_data");
  }

  const size_t idx_group_data = static_cast<size_t>(h5_groups_data.getNumObjs());
  const std::string idx_string = common::formatIndexWithLeadingZeros(idx_group_data, ITER_LENGTH);

  _writer.serializeHdf5(idx_string, group_data, h5_groups_data);

  H5::Group new_h5_group_data = h5_groups_data.openGroup(idx_string);

  _dataset->acquisition.groups_data.push_back(std::move(group_data));

  return {&_dataset->acquisition.groups_data, new_h5_group_data, &_writer};
}

void Stream::readRawData(size_t group_data, const std::shared_ptr<urx::RawData>& raw_data,
                         size_t sequence_urx_raw_data, size_t sequence_h5_raw_data, size_t count) {
  const GroupData& urx_group_data = _dataset->acquisition.groups_data[group_data];
  const GroupDataReader group_data_reader{urx_group_data, 0};
  const RawDataHelper raw_data_helper{raw_data.get()};
  const std::string idx_string = common::formatIndexWithLeadingZeros(group_data, ITER_LENGTH);
  const H5::DataSet dataset = _file.openGroup("dataset")
                                  .openGroup("acquisition")
                                  .openGroup("groups_data")
                                  .openGroup(idx_string)
                                  .openDataSet("raw_data");

  const H5::DataSpace dataspace = dataset.getSpace();
  hsize_t dimension[2];
  dataspace.getSimpleExtentDims(dimension);
  dimension[0] = group_data_reader.offset(1, 0, 0, 0) * count;
  const hsize_t offset[2] = {group_data_reader.offset(sequence_h5_raw_data, 0, 0, 0), 0};
  dataspace.selectHyperslab(H5S_SELECT_SET, dimension, offset);
  const H5::DataSpace memspace(2, dimension);
  dataset.read(
      static_cast<char*>(raw_data->getBuffer()) +
          group_data_reader.offset(sequence_urx_raw_data, 0, 0, 0) * raw_data_helper.sizeofSample(),
      raw_data_helper.getHdf5Type(), memspace, dataspace);
}

}  // namespace urx::utils::io
