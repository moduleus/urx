#include "stream.h"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>
#include <urx/utils/export.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>
#include <urx/utils/io/detail/hdf5.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer_impl.h>
#include <urx/utils/io/writer_options.h>
#include <urx/utils/raw_data_helper.h>

namespace urx::utils::io {

struct GroupDataStream::GroupDataStreamHdf5Fields {
  GroupDataStreamHdf5Fields(const H5::Group& group_data, urx::utils::io::WriterDataset* w)
      : h5_group_data(group_data), writer(w) {}

  H5::Group h5_group_data;
  H5::DataSet h5_dataset_sequence_timestamps;
  H5::Attribute h5_attribute_sequence_timestamps;
  H5::Group h5_event_timestamps;
  H5::DataSet h5_raw_data;
  urx::utils::io::WriterDataset* writer;
};

namespace {
template <typename T>
void extendVector(H5::Group& group, H5::DataSet& dataset, std::vector<T>& vector, const T& value) {
  vector.emplace_back(value);

  const H5::DSetCreatPropList plist = dataset.getCreatePlist();
  const H5D_layout_t layout = plist.getLayout();
  const H5::DataSpace old_space = dataset.getSpace();
  const hsize_t old_size = dataset.getSpace().getSimpleExtentNpoints();
  const H5::PredType& mem_type = *getStdToHdf5().at(urx::utils::nameTypeid<T>());

  if (layout == H5D_CHUNKED) {
    const hsize_t new_size = old_size + 1;
    dataset.extend(&new_size);
    const hsize_t dims[1] = {1};
    old_space.selectHyperslab(H5S_SELECT_SET, dims, &old_size);
    const H5::DataSpace memspace(1, dims);
    dataset.write(&value, mem_type, memspace, old_space);
  } else {
    std::vector<T> data(static_cast<size_t>(old_size));
    if (old_size != 0) {
      dataset.read(data.data(), mem_type);
    }

    data.push_back(value);

    const hsize_t dims[1] = {data.size()};
    const H5::DataSpace new_space(1, dims);
    const std::string dataset_name = dataset.getObjName();

    group.unlink(dataset_name);
    dataset = group.createDataSet(dataset_name, mem_type, new_space);
    dataset.write(data.data(), mem_type);
  }
}

template <typename T>
void extendVector(H5::Group& group, H5::Attribute& attribute, std::vector<T>& vector,
                  const T& value) {
  vector.emplace_back(value);

  const H5::DataSpace old_space = attribute.getSpace();
  const hsize_t old_size = attribute.getSpace().getSimpleExtentNpoints();
  const H5::PredType& mem_type = *getStdToHdf5().at(urx::utils::nameTypeid<T>());

  std::vector<T> data(static_cast<size_t>(old_size));
  if (old_size != 0) {
    attribute.read(mem_type, data.data());
  }

  data.push_back(value);

  const hsize_t dims[1] = {data.size()};
  const H5::DataSpace new_space(1, dims);
  const std::string attribute_name = attribute.getName();

  group.removeAttr(attribute_name);
  attribute = group.createAttribute(attribute_name, mem_type, new_space);
  attribute.write(mem_type, data.data());
}

void extend(H5::Group& group, H5::DataSet& dataset, const std::shared_ptr<RawData>& value,
            urx::utils::io::WriterDataset* writer) {
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

  const urx::DataType dataset_datatype = urx::utils::io::detail::h5PredTypeToDataType(datatype);
  const urx::SamplingType dataset_samplingtype =
      dimension[1] == 1 ? SamplingType::RF : SamplingType::IQ;

  if (urx::utils::io::detail::h5PredTypeToDataType(datatype) != value->getDataType()) {
    throw std::runtime_error("Datatype of RawData to append (" +
                             urx::utils::io::enums::enumToString(value->getDataType()) +
                             ") must have the same data type than the stored RawData (" +
                             urx::utils::io::enums::enumToString(dataset_datatype) + ").");
  }
  if (dataset_samplingtype != value->getSamplingType()) {
    throw std::runtime_error("Sampling type of RawData to append (" +
                             urx::utils::io::enums::enumToString(value->getSamplingType()) +
                             ") must have the same sampling type than the stored RawData (" +
                             urx::utils::io::enums::enumToString(dataset_samplingtype) + ").");
  }

  hsize_t new_dims[2] = {dimension[0] + value->getSize(), dimension[1]};
  // MATLAB doesn't support empty Chunked dataset.
  if (dimension[0] == 0 && layout != H5D_CHUNKED && writer->getOptions().getChunkGroupData()) {
    group.unlink("raw_data");

    writer->serializeHdf5("raw_data", value, group);

    dataset = group.openDataSet("raw_data");
  } else if (layout == H5D_CHUNKED) {
    dataset.extend(new_dims);

    const H5::DataSpace filespace = dataset.getSpace();
    const hsize_t start[2] = {dimension[0], 0};
    const hsize_t count[2] = {value->getSize(), dimension[1]};
    filespace.selectHyperslab(H5S_SELECT_SET, count, start);

    const H5::DataSpace memspace(2, count);
    dataset.write(value->getBuffer(), datatype, memspace, filespace);
  } else {
    const std::shared_ptr<urx::RawData> field = urx::utils::rawDataFactory<urx::RawDataNoInit>(
        urx::utils::io::detail::h5PredTypeToDataType(datatype),
        dimension[1] == 1 ? SamplingType::RF : SamplingType::IQ,
        static_cast<size_t>(dimension[0]) + value->getSize());

    if (dimension[0] != 0) {
      dataset.read(field->getBuffer(), datatype);
    }

    const size_t unitary_size = urx::utils::group_helper::sizeofDataType(value->getDataType()) *
                                (value->getSamplingType() == SamplingType::RF ? 1 : 2);
    memcpy(static_cast<char*>(field->getBuffer()) + unitary_size * dimension[0], value->getBuffer(),
           unitary_size * value->getSize());

    group.unlink("raw_data");

    writer->serializeHdf5("raw_data", field, group);

    dataset = group.openDataSet("raw_data");
  }
}
}  // namespace

GroupDataStream::GroupDataStream() = default;
GroupDataStream::~GroupDataStream() = default;
GroupDataStream::GroupDataStream(GroupDataStream&& other) noexcept = default;
GroupDataStream& GroupDataStream::operator=(GroupDataStream&& other) noexcept = default;

void GroupDataStream::append(const std::shared_ptr<RawData>& raw_data, double sequence_timestamp,
                             const std::vector<double>& event_timestamp) {
  urx::GroupData& group_data = _group_data->at(_group_data_idx);
  group_data.event_timestamps.emplace_back(event_timestamp);

  if (_impl->h5_dataset_sequence_timestamps.getId() != H5I_INVALID_HID) {
    extendVector(_impl->h5_group_data, _impl->h5_dataset_sequence_timestamps,
                 group_data.sequence_timestamps, sequence_timestamp);
  } else if (_impl->h5_attribute_sequence_timestamps.getId() != H5I_INVALID_HID) {
    extendVector(_impl->h5_group_data, _impl->h5_attribute_sequence_timestamps,
                 group_data.sequence_timestamps, sequence_timestamp);
  } else {
    assert(false);
  }

  const std::string idx_string =
      common::formatIndexWithLeadingZeros(group_data.event_timestamps.size() - 1, ITER_LENGTH);

  _impl->writer->serializeHdf5(idx_string, event_timestamp, _impl->h5_event_timestamps);

  if (_impl->h5_raw_data.getId() == H5I_INVALID_HID) {
    _impl->writer->serializeHdf5("raw_data", raw_data, _impl->h5_group_data);

    if (_impl->h5_group_data.nameExists("raw_data")) {
      _impl->h5_raw_data = _impl->h5_group_data.openDataSet("raw_data");
    }
  } else {
    extend(_impl->h5_group_data, _impl->h5_raw_data, raw_data, _impl->writer);
  }
}

URX_UTILS_EXPORT urx::GroupData& GroupDataStream::getGroupData() {
  return (*_group_data)[_group_data_idx];
}

struct Stream::StreamHdf5Fields {
  StreamHdf5Fields(const H5::H5File& file) : h5_file(file) {}

  H5::H5File h5_file;
  urx::utils::io::ReaderDataset reader;
  urx::utils::io::WriterDataset writer;
};

Stream::Stream(const std::string& filename, std::shared_ptr<Dataset> dataset)
    : _impl(new StreamHdf5Fields(H5::H5File{
          filename.c_str(), std::filesystem::exists(filename) ? H5F_ACC_RDWR : H5F_ACC_EXCL})),
      _dataset(std::move(dataset)) {
  _impl->reader.init(*_dataset);
  _impl->writer.init(*_dataset);
}

Stream::~Stream() = default;

std::shared_ptr<Dataset> Stream::getDataset() const { return _dataset; }

void Stream::saveToFile() { _impl->writer.write(_impl->h5_file, *_dataset); }
void Stream::loadFromFile() { _impl->reader.read(_impl->h5_file, *_dataset); }

RawDataLoadPolicy Stream::getRawDataLoadPolicy() const {
  return _impl->reader.getOptions().getRawDataLoadPolicy();
}
void Stream::setRawDataLoadPolicy(RawDataLoadPolicy value) {
  _impl->reader.getOptions().setRawDataLoadPolicy(value);
}

bool Stream::getChunkGroupData() const { return _impl->writer.getOptions().getChunkGroupData(); }
void Stream::setChunkGroupData(bool value) { _impl->writer.getOptions().setChunkGroupData(value); }

const ReaderOptions& Stream::readerOptions() const { return _impl->reader.getOptions(); }
ReaderOptions& Stream::readerOptions() { return _impl->reader.getOptions(); }
const WriterOptions& Stream::writerOptions() const { return _impl->writer.getOptions(); }
WriterOptions& Stream::writerOptions() { return _impl->writer.getOptions(); }

GroupDataStream Stream::createGroupData(const std::shared_ptr<Group>& group, DoubleNan timestamp) {
  urx::GroupData group_data;
  group_data.group = group;
  group_data.raw_data =
      urx::utils::rawDataFactory<RawDataNoInit>(group->data_type, group->sampling_type, 0);
  group_data.group_timestamp = timestamp;

  const H5::Group h5_acquisition = _impl->h5_file.openGroup("dataset").openGroup("acquisition");

  H5::Group h5_groups_data;
  if (!h5_acquisition.nameExists("groups_data")) {
    h5_groups_data = h5_acquisition.createGroup("groups_data");
  } else {
    h5_groups_data = h5_acquisition.openGroup("groups_data");
  }

  const size_t idx_group_data = static_cast<size_t>(h5_groups_data.getNumObjs());
  const std::string idx_string = common::formatIndexWithLeadingZeros(idx_group_data, ITER_LENGTH);

  _impl->writer.serializeHdf5(idx_string, group_data, h5_groups_data);

  const H5::Group new_h5_group_data = h5_groups_data.openGroup(idx_string);

  _dataset->acquisition.groups_data.push_back(std::move(group_data));

  GroupDataStream retval;

  retval._impl = std::make_unique<urx::utils::io::GroupDataStream::GroupDataStreamHdf5Fields>(
      new_h5_group_data, &_impl->writer);
  retval._group_data = &_dataset->acquisition.groups_data;
  retval._group_data_idx = _dataset->acquisition.groups_data.size() - 1;

  if (retval._impl->h5_group_data.nameExists("sequence_timestamps")) {
    retval._impl->h5_dataset_sequence_timestamps =
        retval._impl->h5_group_data.openDataSet("sequence_timestamps");
  } else if (retval._impl->h5_group_data.attrExists("sequence_timestamps")) {
    retval._impl->h5_attribute_sequence_timestamps =
        retval._impl->h5_group_data.openAttribute("sequence_timestamps");
  } else {
    throw std::runtime_error("Failed to read " + retval._impl->h5_group_data.getObjName() +
                             "/sequence_timestamps");
  }

  retval._impl->h5_event_timestamps = retval._impl->h5_group_data.openGroup("event_timestamps");
  if (retval._impl->h5_group_data.nameExists("raw_data")) {
    retval._impl->h5_raw_data = retval._impl->h5_group_data.openDataSet("raw_data");
  }

  return retval;
}

void Stream::readRawData(size_t group_data, const std::shared_ptr<urx::RawData>& raw_data,
                         size_t sequence_urx_raw_data, size_t sequence_h5_raw_data, size_t count) {
  const GroupData& urx_group_data = _dataset->acquisition.groups_data[group_data];
  const GroupDataReader group_data_reader{urx_group_data};
  const RawDataHelper raw_data_helper{raw_data.get()};
  const std::string idx_string = common::formatIndexWithLeadingZeros(group_data, ITER_LENGTH);
  const H5::DataSet dataset = _impl->h5_file.openGroup("dataset")
                                  .openGroup("acquisition")
                                  .openGroup("groups_data")
                                  .openGroup(idx_string)
                                  .openDataSet("raw_data");

  const H5::DataSpace dataspace = dataset.getSpace();
  hsize_t dimension[2];
  dataspace.getSimpleExtentDims(dimension);
  if ((dimension[1] == 1 && raw_data->getSamplingType() != urx::SamplingType::RF) ||
      (dimension[1] == 2 && raw_data->getSamplingType() != urx::SamplingType::IQ)) {
    throw std::runtime_error("Incompatible dimension between raw_data and hdf5 data.");
  }
  dimension[0] = group_data_reader.offset(1, 0, 0, 0) * count;
  const hsize_t offset[2] = {group_data_reader.offset(sequence_h5_raw_data, 0, 0, 0), 0};
  dataspace.selectHyperslab(H5S_SELECT_SET, dimension, offset);
  const H5::DataSpace memspace(2, dimension);
  dataset.read(
      static_cast<char*>(raw_data->getBuffer()) +
          group_data_reader.offset(sequence_urx_raw_data, 0, 0, 0) * raw_data_helper.sizeofSample(),
      urx::utils::io::detail::dataTypeToH5PredType(raw_data->getDataType()), memspace, dataspace);
}

void Stream::readRawData(size_t group_data, void* raw_data, DataType data_type,
                         SamplingType sampling_type, size_t sequence_urx_raw_data,
                         size_t sequence_h5_raw_data, size_t count) {
  const GroupData& urx_group_data = _dataset->acquisition.groups_data[group_data];
  const GroupDataReader group_data_reader{urx_group_data};
  const std::string idx_string = common::formatIndexWithLeadingZeros(group_data, ITER_LENGTH);
  const H5::DataSet dataset = _impl->h5_file.openGroup("dataset")
                                  .openGroup("acquisition")
                                  .openGroup("groups_data")
                                  .openGroup(idx_string)
                                  .openDataSet("raw_data");

  const H5::DataSpace dataspace = dataset.getSpace();
  hsize_t dimension[2];
  dataspace.getSimpleExtentDims(dimension);
  if ((dimension[1] == 1 && sampling_type != urx::SamplingType::RF) ||
      (dimension[1] == 2 && sampling_type != urx::SamplingType::IQ)) {
    throw std::runtime_error("Incompatible dimension between raw_data and hdf5 data.");
  }
  dimension[0] = group_data_reader.offset(1, 0, 0, 0) * count;
  const hsize_t offset[2] = {group_data_reader.offset(sequence_h5_raw_data, 0, 0, 0), 0};
  dataspace.selectHyperslab(H5S_SELECT_SET, dimension, offset);
  const H5::DataSpace memspace(2, dimension);
  dataset.read(static_cast<char*>(raw_data) +
                   group_data_reader.offset(sequence_urx_raw_data, 0, 0, 0) *
                       urx::utils::group_helper::sizeofSample(sampling_type, data_type),
               urx::utils::io::detail::dataTypeToH5PredType(data_type), memspace, dataspace);
}

}  // namespace urx::utils::io
