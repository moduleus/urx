#include <cassert>
#include <cstdio>

#include <uff/uff.h>
#include <uff/vector.h>
#include <uff_utils/io.h>
#include <uff_utils/writer.h>

namespace uff {

/**
 * Return the string-ID (format: "00000123") of the pointer wptr.
 * The string-ID is the position of the pointer in the vector 'vec'. First pointer's string-ID is "00000001".
 * If 'wptr' does not point to an object stored in 'vec' then return "????????"
 */

template <typename T>
std::string Writer::getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec,
                                     const std::shared_ptr<T>& wptr) {
  if (auto p1 = wptr.lock()) {
    int cnt = 1;
    for (const auto& p2 : vec) {
      if (p1 == p2) {
        char buf[9];
        snprintf(buf, sizeof buf, "%08d", cnt);
        return {buf};
      }
      cnt++;
    }
  }

  assert(false);
  return "????????";
}

void Writer::writeToFile() {
  H5::Exception::dontPrint();

  const H5::H5File file(_filename, H5F_ACC_TRUNC);

  // Version
  const H5::Group version(file.createGroup("version"));
  // writeVersion(version, _dataset->version());

  // Channel Data
  const H5::Group channelData(file.createGroup("channel_data"));
  // writeChannelData(channelData, _dataset->channelData());
}

void Writer::writeMetadataTypeDataset(const H5::Group& group, const std::string& name,
                                      double value) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
}

void Writer::writeOptionalMetadataTypeDataset(const H5::Group& group, const std::string& name,
                                              const std::optional<double>& value) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  if (value.has_value()) {
    dataset.write(&value.value(), datatype, dataspace);
  } else {
    dataset.write(&UFF_NAN, datatype, dataspace);
  }
}

void Writer::writePoint2D(const H5::Group& group, const Vector2D<double>& position) {
  writeMetadataTypeDataset(group, "x", position.x);
  writeMetadataTypeDataset(group, "y", position.y);
}

void Writer::writePoint3D(const H5::Group& group, const Vector3D<double>& position) {
  writeMetadataTypeDataset(group, "x", position.x);
  writeMetadataTypeDataset(group, "y", position.y);
  writeMetadataTypeDataset(group, "z", position.z);
}

void Writer::writeDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                       const std::vector<DataType>& values,
                                       const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5DataType, dataspace);
  dataset.write(values.data(), H5DataType);
}

void Writer::writeMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                           const std::vector<double>& values,
                                           const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5MetadataType, dataspace);
  dataset.write(values.data(), H5MetadataType);
}

void Writer::writeIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                      const std::vector<int>& values,
                                      const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_INT, dataspace);
  dataset.write(values.data(), H5::PredType::NATIVE_INT);
}

void Writer::writeIntegerDataset(const H5::Group& group, const std::string& name, int value) {
  const H5::StrType datatype(H5::PredType::NATIVE_INT);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
}

}  // namespace uff
