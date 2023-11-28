#include "writer.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iosfwd>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include <version>

#include <H5Cpp.h>
#include <boost/pfr.hpp>
#include <magic_enum.hpp>

#include <uff/acquisition.h>
#include <uff/dataset.h>
#include <uff/detail/double_nan.h>
#include <uff/element_geometry.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/impulse_response.h>
#include <uff/probe.h>
#include <uff/wave.h>

namespace uff {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

std::unordered_map<std::type_index, const H5::PredType*> std_to_h5;

constexpr int iter_length = 8;

template <typename T>
void serialize_all(const T& field, const H5::Group& group, MapToSharedPtr& map);
template <>
void serialize_all(const GroupData::VecDataTypeVariant& field, const H5::Group& group,
                   MapToSharedPtr& map);

// Default
template <typename T>
void serialize_hdf5(const std::string& name, const T& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  const H5::Group group_child(group.createGroup(name));

  serialize_all(field, group_child, map);
}

// Number
template <typename T>
concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;
template <Number T>
void serialize_hdf5(const std::string& name, const T& field, const H5::Group& group,
                    MapToSharedPtr&) {
  const H5::StrType datatype(*std_to_h5.at(typeid(T)));
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&field, datatype, dataspace);
}

// String
template <>
void serialize_hdf5(const std::string& name, const std::string& field, const H5::Group& group,
                    MapToSharedPtr&) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(field, datatype, dataspace);
}

// DoubleNan
template <>
void serialize_hdf5(const std::string& name, const DoubleNan& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  serialize_hdf5(name, field.value, group, map);
}

// GroupData::VecDataTypeVariant
template <>
void serialize_hdf5(const std::string&, const GroupData::VecDataTypeVariant& field,
                    const H5::Group& group, MapToSharedPtr& map) {
  serialize_all(field, group, map);
}

// shared_ptr
template <typename T>
void serialize_hdf5(const std::string& name, const std::shared_ptr<T>& field,
                    const H5::Group& group, MapToSharedPtr& map) {
  serialize_hdf5(name, *field, group, map);
}

// weak_ptr
template <typename T>
void serialize_hdf5(const std::string& name, const std::weak_ptr<T>& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  if (auto shared = field.lock()) {
    const std::vector<std::shared_ptr<T>>& all_shared =
        *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(
            map.at(std::type_index{typeid(T)}));
    auto idx = std::find_if(
        all_shared.begin(), all_shared.end(),
        [&shared](const std::shared_ptr<T>& data) { return shared.get() == data.get(); });
    if (idx == all_shared.end()) {
      throw std::runtime_error(("Failed to read data from " + name).c_str());
    }
    serialize_hdf5(name, std::distance(all_shared.begin(), idx), group, map);
  } else {
    throw std::runtime_error(("Failed to read data from " + name).c_str());
  }
}

// Vector
template <typename T>
void serialize_hdf5(const std::string& name, const std::vector<T>& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  if constexpr (Number<T>) {
    const size_t size = field.size();
    const hsize_t dims[1] = {size};
    const H5::DataSpace dataspace = H5::DataSpace(1, dims);
    const H5::PredType* datatype = std_to_h5.at(typeid(T));
    const H5::DataSet dataset = group.createDataSet(name, *datatype, dataspace);
    dataset.write(field.data(), *datatype);
  } else {
    const H5::Group group_child(group.createGroup(name));

    size_t i = 0;
    for (const auto& iter : field) {
      std::stringstream stream;
      stream << std::setfill('0') << std::setw(iter_length) << i;
      serialize_hdf5(stream.str(), iter, group_child, map);
      i++;
    }
  }
}

// Enumeration
template <typename T>
concept Enum = std::is_enum_v<T>;
template <Enum T>
void serialize_hdf5(const std::string& name, const T& field, const H5::Group& group,
                    MapToSharedPtr&) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  const std::string_view sv = magic_enum::enum_name(field);
  const std::string value = sv.empty() ? std::to_string(static_cast<int>(field)) : std::string{sv};
  dataset.write(value, datatype, dataspace);
}

template <typename T>
void serialize_all(const T& field, const H5::Group& group, MapToSharedPtr& map) {
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
  boost::pfr::for_each_field(field, [&a, &group, &map](const auto& child, std::size_t idx) {
    serialize_hdf5(std::string{a[idx]}, child, group, map);
  });
}

// Need to update map for Probe.
template <>
void serialize_all(const Probe& field, const H5::Group& group, MapToSharedPtr& map) {
  map.insert({typeid(ElementGeometry), &field.element_geometries});
  map.insert({typeid(ImpulseResponse), &field.impulse_responses});
  auto a = boost::pfr::names_as_array<Probe>();
  boost::pfr::for_each_field(field, [&a, &group, &map](const auto& child, std::size_t idx) {
    serialize_hdf5(std::string{a[idx]}, child, group, map);
  });
  map.erase(typeid(ElementGeometry));
  map.erase(typeid(ImpulseResponse));
}

// for_each_field doesn't support std::variant.
template <>
void serialize_all(const GroupData::VecDataTypeVariant& field, const H5::Group& group,
                   MapToSharedPtr&) {
  enum class Format { ARRAY_2D, COMPOUND };
  constexpr Format format = Format::ARRAY_2D;

  const auto [size, datatype, data] = std::visit(
      [](const auto& vec) {
        const size_t size = vec.size();
        const H5::PredType* datatype;

        using type = typename std::remove_cvref_t<decltype(vec)>::value_type;
        if constexpr (Number<type>) {
          datatype = std_to_h5.at(typeid(type));
        } else
        // Complex
        {
          using type2 = typename type::value_type;
          datatype = std_to_h5.at(typeid(type2));
        }

        const void* data = static_cast<const void*>(vec.data());

        return std::tuple(size, datatype, data);
      },
      field);

  if constexpr (format == Format::ARRAY_2D) {
    const bool isComplex = std::visit(
        [](const auto& vec) {
          using type = typename std::remove_cvref_t<decltype(vec)>::value_type;
          return !Number<type>;
        },
        field);
    hsize_t dims[2] = {size, isComplex ? 2ULL : 1ULL};
    const H5::DataSpace dataspace = H5::DataSpace(2, dims);
    const H5::DataSet dataset = group.createDataSet("raw_data", *datatype, dataspace);
    dataset.write(data, *datatype);
  } else {
    hsize_t dims[1] = {size};
    const H5::DataSpace dataspace = H5::DataSpace(1, dims);

    const size_t sizeOf = std::visit(
        [](const auto& vec) {
          return sizeof(typename std::remove_cvref_t<decltype(vec)>::value_type);
        },
        field);

    H5::CompType complexType(sizeOf);

    std::visit(
        [&complexType, datatype](const auto& vec) {
          using type = typename std::remove_cvref_t<decltype(vec)>::value_type;
          if constexpr (Number<type>) {
            complexType.insertMember("real", 0ULL, *datatype);
          } else {
            using type2 = typename type::value_type;
            complexType.insertMember("real", 0ULL, *datatype);
            complexType.insertMember("imag", sizeof(type2), *datatype);
          }
        },
        field);
    const H5::DataSet dataset = group.createDataSet("raw_data", complexType, dataspace);
    dataset.write(data, complexType);
  }
}
}  // namespace

void Writer::saveToFile(const std::string& filename, const Dataset& dataset) {
  const H5::H5File file(filename.data(), H5F_ACC_TRUNC);
  MapToSharedPtr map_to_shared_ptr{{typeid(Group), &dataset.acquisition.groups},
                                   {typeid(Probe), &dataset.acquisition.probes},
                                   {typeid(Wave), &dataset.acquisition.waves},
                                   {typeid(Excitation), &dataset.acquisition.excitations},
                                   {typeid(GroupData), &dataset.acquisition.groups_data}};

  serialize_hdf5("dataset", dataset, file, map_to_shared_ptr);
}

}  // namespace uff
