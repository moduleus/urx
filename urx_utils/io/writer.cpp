#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
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
#include <urx_utils/io/common.h>
#include <urx_utils/io/writer.h>

namespace urx {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

std::unordered_map<std::type_index, const H5::PredType*> std_to_h5;

constexpr int iter_length = 8;
//constexpr bool use_attribute = false;

//template <typename T>
//void save() {}

template <typename T>
void serialize_all(const T& field, const H5::Group& group, MapToSharedPtr& map);

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
  const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
  attribute.write(datatype, &field);
}

// String
template <>
void serialize_hdf5(const std::string& name, const std::string& field, const H5::Group& group,
                    MapToSharedPtr&) {
  const H5::StrType datatype(0, H5T_VARIABLE);
  const H5::DataSpace dataspace(H5S_SCALAR);
  const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
  attribute.write(datatype, field);
}

// DoubleNan
template <>
void serialize_hdf5(const std::string& name, const DoubleNan& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  serialize_hdf5(name, field.value, group, map);
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
  // Never assigned
  if (!field.owner_before(std::weak_ptr<T>{}) && !std::weak_ptr<T>{}.owner_before(field)) {
    return;
  }

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
    const H5::Attribute attribute = group.createAttribute(name, *datatype, dataspace);
    if (size != 0) {
      attribute.write(*datatype, field.data());
    }
  } else {
    const H5::Group group_child(group.createGroup(name));

    size_t i = 0;
    for (const auto& iter : field) {
      serialize_hdf5(format_index_with_leading_zeros(i, iter_length), iter, group_child, map);
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
  const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
  const std::string_view sv = magic_enum::enum_name(field);
  const std::string value = sv.empty() ? std::to_string(static_cast<int>(field)) : std::string{sv};
  attribute.write(datatype, value);
}

void serialize_all(const std::shared_ptr<Group>& gr, const std::shared_ptr<RawData>& field,
                   const H5::Group& group, MapToSharedPtr&);

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
  boost::pfr::for_each_field(field, [&a, &group, &map, &field](const auto& child, std::size_t idx) {
    if constexpr (std::is_same_v<std::remove_cvref_t<decltype(child)>, std::shared_ptr<RawData>> &&
                  std::is_same_v<std::remove_cvref_t<T>, GroupData>) {
      serialize_all(reinterpret_cast<const GroupData&>(field).group.lock(), child, group, map);
    } else {
      serialize_hdf5(std::string{a[idx]}, child, group, map);
    }
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
void serialize_all(const std::shared_ptr<Group>& gr, const std::shared_ptr<RawData>& field,
                   const H5::Group& group, MapToSharedPtr&) {
  enum class Format { ARRAY_2D, COMPOUND };
  constexpr Format format = Format::ARRAY_2D;

  static std::unordered_map<Group::DataType, std::type_index> group_dt_to_typeid{
      {Group::DataType::INT16, typeid(int16_t)},
      {Group::DataType::INT32, typeid(int32_t)},
      {Group::DataType::FLOAT, typeid(float)},
      {Group::DataType::DOUBLE, typeid(double)}};
  static std::unordered_map<Group::DataType, size_t> group_dt_to_sizeof{
      {Group::DataType::INT16, sizeof(int16_t)},
      {Group::DataType::INT32, sizeof(int32_t)},
      {Group::DataType::FLOAT, sizeof(float)},
      {Group::DataType::DOUBLE, sizeof(double)}};

  const H5::PredType* datatype = std_to_h5.at(group_dt_to_typeid.at(gr->data_type));

  const bool isComplex = gr->sampling_type == Group::SamplingType::IQ;
  if constexpr (format == Format::ARRAY_2D) {
    const hsize_t dims[2] = {field->getSize(), isComplex ? 2ULL : 1ULL};
    const H5::DataSpace dataspace = H5::DataSpace(2, dims);
    const H5::DataSet dataset = group.createDataSet("raw_data", *datatype, dataspace);
    dataset.write(field->getBuffer(), *datatype);
  } else {
    const hsize_t dims[1] = {field->getSize()};
    const H5::DataSpace dataspace = H5::DataSpace(1, dims);

    const size_t sizeOf = group_dt_to_sizeof.at(gr->data_type);

    const H5::CompType complexType(sizeOf * (isComplex ? 2 : 1));

    if (isComplex) {
      complexType.insertMember("real", 0ULL, *datatype);
      complexType.insertMember("imag", sizeOf, *datatype);
    } else {
      complexType.insertMember("real", 0ULL, *datatype);
    }
    const H5::DataSet dataset = group.createDataSet("raw_data", complexType, dataspace);
    dataset.write(field->getBuffer(), complexType);
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

}  // namespace urx
