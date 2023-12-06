#include <array>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <version>

#include <H5Cpp.h>
#include <boost/pfr.hpp>
#include <magic_enum.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element_geometry.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/wave.h>
#include <urx_utils/io/reader.h>

namespace urx {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

std::unordered_map<std::type_index, const H5::PredType*> std_to_h5;

constexpr int iter_length = 8;

template <typename T>
void deserialize_all(T& field, const H5::Group& group, MapToSharedPtr& map);
template <>
void deserialize_all(std::shared_ptr<RawData>& field, const H5::Group& group, MapToSharedPtr& map);

// Default
template <typename T>
void deserialize_hdf5(const std::string& name, T& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  deserialize_all(field, group_child, map);
}

// Number
template <class T>
concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;
template <Number T>
void deserialize_hdf5(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr&) {
  const H5::StrType datatype(*std_to_h5.at(typeid(T)));
  const H5::DataSet dataset = group.openDataSet(name);
  dataset.read(&field, datatype);
}

// String
template <>
void deserialize_hdf5(const std::string& name, std::string& field, const H5::Group& group,
                      MapToSharedPtr&) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.openDataSet(name);
  dataset.read(field, datatype, dataspace);
}

// DoubleNan
template <>
void deserialize_hdf5(const std::string& name, DoubleNan& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  deserialize_hdf5(name, field.value, group, map);
}

// shared_ptr
template <typename T>
void deserialize_hdf5(const std::string& name, std::shared_ptr<T>& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  field = std::make_shared<T>();
  deserialize_hdf5(name, *field, group, map);
}

// RawData
template <>
void deserialize_hdf5(const std::string&, std::shared_ptr<RawData>& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  deserialize_all(field, group, map);
}

// weak_ptr
template <typename T>
void deserialize_hdf5(const std::string& name, std::weak_ptr<T>& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  if (group.exists(name)) {
    std::size_t idx;

    deserialize_hdf5(name, idx, group, map);

    const std::vector<std::shared_ptr<T>>& all_shared =
        *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(
            map.at(std::type_index{typeid(T)}));

    field = all_shared[idx];
  }
}

// Vector
template <typename T>
void deserialize_hdf5(const std::string& name, std::vector<T>& field, const H5::Group& group,
                      MapToSharedPtr& map) {
  if constexpr (Number<T>) {
    const H5::DataSet dataset = group.openDataSet(name);
    const H5::StrType datatype(*std_to_h5.at(typeid(T)));
    const H5::DataSpace dataspace = dataset.getSpace();
    const int ndims = dataspace.getSimpleExtentNdims();
    std::vector<hsize_t> dimension;
    dimension.resize(ndims);
    dataspace.getSimpleExtentDims(dimension.data());
    field.resize(dimension[0]);
    dataset.read(field.data(), datatype);
  } else {
    const H5::Group group_child(group.openGroup(name));

    for (size_t i = 0; i < group_child.getNumObjs(); i++) {
      field.push_back(T{});
      deserialize_hdf5(std::format("{:0{}}", i, iter_length), field.back(), group_child, map);
    }
  }
}

// Enumeration
template <typename T>
concept Enum = std::is_enum_v<T>;
template <Enum T>
void deserialize_hdf5(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr&) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.openDataSet(name);
  std::string value;
  dataset.read(value, datatype, dataspace);

  std::optional<T> convert = magic_enum::enum_cast<T>(value);
  if (convert) {
    field = *convert;
  } else {
    field = static_cast<T>(stoi(value));
  }
}

template <typename T>
void deserialize_all(T& field, const H5::Group& group, MapToSharedPtr& map) {
  if (std_to_h5.empty()) {
    std_to_h5 = {{typeid(char), &H5::PredType::NATIVE_CHAR},
                 {typeid(signed char), &H5::PredType::NATIVE_SCHAR},
                 {typeid(unsigned char), &H5::PredType::NATIVE_UCHAR},
                 {typeid(short), &H5::PredType::NATIVE_SHORT},
                 {typeid(unsigned short), &H5::PredType::NATIVE_USHORT},
                 {typeid(int), &H5::PredType::NATIVE_INT},
                 {typeid(unsigned int), &H5::PredType::NATIVE_UINT},
                 {typeid(long), &H5::PredType::NATIVE_LONG},
                 {typeid(unsigned long), &H5::PredType::NATIVE_ULONG},
                 {typeid(long long), &H5::PredType::NATIVE_LLONG},
                 {typeid(unsigned long long), &H5::PredType::NATIVE_ULLONG},
                 {typeid(float), &H5::PredType::NATIVE_FLOAT},
                 {typeid(double), &H5::PredType::NATIVE_DOUBLE},
                 {typeid(long double), &H5::PredType::NATIVE_LDOUBLE},
                 {typeid(std::int8_t), &H5::PredType::NATIVE_INT8},
                 {typeid(std::uint8_t), &H5::PredType::NATIVE_UINT8},
                 {typeid(std::int16_t), &H5::PredType::NATIVE_INT16},
                 {typeid(std::uint16_t), &H5::PredType::NATIVE_UINT16},
                 {typeid(std::int32_t), &H5::PredType::NATIVE_INT32},
                 {typeid(std::uint32_t), &H5::PredType::NATIVE_UINT32},
                 {typeid(std::int64_t), &H5::PredType::NATIVE_INT64},
                 {typeid(std::uint64_t), &H5::PredType::NATIVE_UINT64},
                 {typeid(std::size_t), &H5::PredType::NATIVE_UINT64}};
  }

  auto a = boost::pfr::names_as_array<T>();
  boost::pfr::for_each_field(field, [&a, &group, &map](auto& child, std::size_t idx) {
    deserialize_hdf5(std::string{a[idx]}, child, group, map);
  });
}

// Need to update map for Probe.
template <>
void deserialize_all(Probe& field, const H5::Group& group, MapToSharedPtr& map) {
  map.insert({typeid(ElementGeometry), &field.element_geometries});
  map.insert({typeid(ImpulseResponse), &field.impulse_responses});
  auto a = boost::pfr::names_as_array<Probe>();
  boost::pfr::for_each_field(field, [&a, &group, &map](auto& child, std::size_t idx) {
    deserialize_hdf5(std::string{a[idx]}, child, group, map);
  });
  map.erase(typeid(ElementGeometry));
  map.erase(typeid(ImpulseResponse));
}

// for_each_field doesn't support RawData.
template <>
void deserialize_all(std::shared_ptr<RawData>& field, const H5::Group& group, MapToSharedPtr&) {
  const H5::DataSet dataset = group.openDataSet("raw_data");
  const H5::DataSpace dataspace = dataset.getSpace();
  const H5::DataType datatype_raw = dataset.getDataType();
  const H5::DataType datatype = [&dataset, &datatype_raw]() {
    if (datatype_raw.getClass() == H5T_COMPOUND) {
      return dataset.getCompType().getMemberDataType(0);
    }
    return datatype_raw;
  }();

  const int ndims = dataspace.getSimpleExtentNdims();
  if ((datatype_raw.getClass() == H5T_COMPOUND && ndims != 1) ||
      (datatype_raw.getClass() != H5T_COMPOUND && ndims != 2)) {
    throw std::runtime_error("Invalid dimension of raw_data");
  }
  std::vector<hsize_t> dimension;
  dimension.resize(2);
  dataspace.getSimpleExtentDims(dimension.data());
  if (datatype_raw.getClass() == H5T_COMPOUND) {
    dimension[1] = dataset.getCompType().getNmembers();
  }

  if (datatype == H5::PredType::NATIVE_INT16) {
    if (dimension[1] == 1) {
      field = std::make_shared<RawDataNoInit<int16_t>>(dimension[0]);
    } else {
      field = std::make_shared<RawDataNoInit<std::complex<int16_t>>>(dimension[0]);
    }
  } else if (datatype == H5::PredType::NATIVE_INT32) {
    if (dimension[1] == 1) {
      field = std::make_shared<RawDataNoInit<int32_t>>(dimension[0]);
    } else {
      field = std::make_shared<RawDataNoInit<std::complex<int32_t>>>(dimension[0]);
    }
  } else if (datatype == H5::PredType::NATIVE_FLOAT) {
    if (dimension[1] == 1) {
      field = std::make_shared<RawDataNoInit<float>>(dimension[0]);
    } else {
      field = std::make_shared<RawDataNoInit<std::complex<float>>>(dimension[0]);
    }
  } else if (datatype == H5::PredType::NATIVE_DOUBLE) {
    if (dimension[1] == 1) {
      field = std::make_shared<RawDataNoInit<double>>(dimension[0]);
    } else {
      field = std::make_shared<RawDataNoInit<std::complex<double>>>(dimension[0]);
    }
  } else
    throw std::runtime_error("Invalid format of raw_data");

  dataset.read(field->getBuffer(), datatype_raw);
}
}  // namespace

std::shared_ptr<Dataset> Reader::loadFromFile(const std::string& filename) {
  // OK
  auto dataset = std::make_shared<Dataset>();

  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);
  MapToSharedPtr map_to_shared_ptr{{typeid(Group), &dataset->acquisition.groups},
                                   {typeid(Probe), &dataset->acquisition.probes},
                                   {typeid(Wave), &dataset->acquisition.waves},
                                   {typeid(Excitation), &dataset->acquisition.excitations},
                                   {typeid(GroupData), &dataset->acquisition.groups_data}};

  deserialize_hdf5("dataset", *dataset, file, map_to_shared_ptr);

  return dataset;
}

}  // namespace urx
