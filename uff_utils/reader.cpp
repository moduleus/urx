#include <uff/dataset.h>
#include <uff/uff.h>
#include <uff_utils/reader.h>
#include <cmath>
#include <cstdio>
#include <utility>

#include "io.h"

namespace uff {

void Reader::updateMetadata() {
  _dataset = std::make_shared<Dataset>();

  H5::Exception::dontPrint();

  const H5::H5File file(_filename, H5F_ACC_RDONLY);

  // Version
  const H5::Group version(file.openGroup("version"));
  // _dataset->setVersion(readVersion(version));

  // Channel Data
  // H5::Group channelData(file.openGroup("channel_data"));
  // _dataset->setChannelData(readChannelData(channelData, false, _skip_data));
}

std::shared_ptr<Dataset> Reader::loadFile(std::string_view /*filename*/, bool /*castData*/,
                                          bool /*skipChannelData*/) {
  // auto dataset = std::make_shared<Dataset<DataType>>();
  // dataset->channelData().setSkipChannelDataData(skipChannelData);

  // H5::Exception::dontPrint();

  // H5::H5File file(filename.data(), H5F_ACC_RDONLY);

  // // Version
  // H5::Group version(file.openGroup("version"));
  // dataset->setVersion(readVersion(version));

  // // Channel Data
  // H5::Group channelData(file.openGroup("channel_data"));
  // dataset->setChannelData(readChannelData(channelData, castData, skipChannelData));

  // return dataset;

  return std::make_shared<Dataset>();
}

double Reader::readMetadataTypeDataset(const H5::Group& group, const std::string& name) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSet dataset = group.openDataSet(name);
  double value;
  dataset.read(&value, datatype);
  return value;
}

std::optional<double> Reader::readOptionalMetadataTypeDataset(const H5::Group& group,
                                                              const std::string& name) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSet dataset = group.openDataSet(name);
  double value;
  dataset.read(&value, datatype);
  std::optional<double> result = std::nullopt;
  if (!std::isnan(value)) {
    result = value;
  }
  return result;
}

void Reader::readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                      std::vector<DataType>& values,
                                      std::vector<hsize_t>& dimensions,
                                      const H5::PredType& targetType, bool skipChannelData) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(targetType);

  // find dataset dimensions
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  if (!skipChannelData) {
    // reserve space in the output buffer
    values.resize(numel);

    // read data
    dataset.read(values.data(), datatype);
  }
}

void Reader::readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                          std::vector<double>& values,
                                          std::vector<hsize_t>& dimensions) {
  const H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  const H5::StrType datatype(H5MetadataType);

  // find dataset dimensions
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  // reserve space in the output buffer
  values.resize(numel);

  // read data
  dataset.read(values.data(), datatype);
}

void Reader::readIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                     std::vector<int>& values, std::vector<hsize_t>& dimensions) {
  const H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  const H5::StrType datatype(H5::PredType::NATIVE_INT);

  // find dataset dimensions
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  // reserve space in the output buffer
  values.resize(numel);

  // read data
  dataset.read(values.data(), datatype);
}

int Reader::readIntegerDataset(const H5::Group& group, const std::string& name) {
  const H5::StrType datatype(H5::PredType::NATIVE_INT);
  const H5::DataSet dataset = group.openDataSet(name);
  int value;
  dataset.read(&value, datatype);
  return value;
}

std::string Reader::readStringDataset(const H5::Group& group, const std::string& name) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.openDataSet(name);
  std::string buffer;
  dataset.read(buffer, datatype, dataspace);
  return buffer;
}

std::optional<std::string> Reader::readOptionalStringDataset(const H5::Group& group,
                                                             const std::string& name) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.openDataSet(name);
  std::string buffer;
  dataset.read(buffer, datatype, dataspace);
  std::optional<std::string> result = std::nullopt;
  if (buffer != "undefined") result = buffer;
  return result;
}

}  // namespace uff
