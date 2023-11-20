#include "writer.hpp"
#include <H5Cpp.h>
#include <boost/pfr.hpp>
#include <magic_enum.hpp>
#include <typeindex>
#include <utility>

namespace uff {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

std::unordered_map<std::type_index, const H5::PredType*> std_to_h5 = {
    {typeid(char), &H5::PredType::NATIVE_CHAR},
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
template <class T>
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
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(field, datatype, dataspace);
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
      throw std::exception(("Failed to read data from " + name).c_str());
    }
    serialize_hdf5(name, std::distance(all_shared.begin(), idx), group, map);
  } else {
    throw std::exception(("Failed to read data from " + name).c_str());
  }
}

// Vector
template <typename T>
void serialize_hdf5(const std::string& name, const std::vector<T>& field, const H5::Group& group,
                    MapToSharedPtr& map) {
  if constexpr (Number<T>) {
    size_t size = field.size();
    hsize_t dims[1] = {size};
    H5::DataSpace dataspace = H5::DataSpace(1, dims);
    const H5::PredType* datatype = std_to_h5.at(typeid(T));
    H5::DataSet dataset = group.createDataSet(name, *datatype, dataspace);
    dataset.write(field.data(), *datatype);
  } else {
    const H5::Group group_child(group.createGroup(name));

    size_t i = 0;
    int length =
        field.size() == 1 ? 0 : static_cast<int>(std::trunc(std::log10(field.size() - 1))) + 1;
    for (const auto& iter : field) {
      std::stringstream stream;
      stream << std::setfill('0') << std::setw(length) << i;
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
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(std::string{magic_enum::enum_name(field)}, datatype, dataspace);
}

template <typename T>
void serialize_all(const T& field, const H5::Group& group, MapToSharedPtr& map) {
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
                   MapToSharedPtr& map) {
  size_t size = std::visit(
      [](const auto& vec) {
        using type = std::remove_cvref_t<decltype(vec)>::value_type;
        if constexpr (Number<type>) {
          return vec.size();
        } else
        // Complex
        {
          return vec.size() * 2;
        }
      },
      field);
  hsize_t dims[1] = {size};
  H5::DataSpace dataspace = H5::DataSpace(1, dims);

  const H5::PredType* datatype = std::visit(
      [](const auto& vec) {
        using type = std::remove_cvref_t<decltype(vec)>::value_type;
        if constexpr (Number<type>) {
          return std_to_h5.at(typeid(type));
        } else
        // Complex
        {
          using type2 = type::value_type;
          return std_to_h5.at(typeid(type2));
        }
      },
      field);
  H5::DataSet dataset = group.createDataSet("raw_data", *datatype, dataspace);
  const void* data =
      std::visit([](const auto& vec) { return static_cast<const void*>(vec.data()); }, field);
  dataset.write(data, *datatype);
}
}  // namespace

void Writer::saveToFile(const std::string& filename, const Dataset& dataset) {
  const H5::H5File file(filename.data(), H5F_ACC_TRUNC);
  MapToSharedPtr map_to_shared_ptr{{typeid(Group), &dataset.acquisition.groups},
                                   {typeid(Probe), &dataset.acquisition.probes},
                                   {typeid(Wave), &dataset.acquisition.waves},
                                   {typeid(Excitation), &dataset.acquisition.excitations},
                                   {typeid(GroupData), &dataset.acquisition.groups_data}};

  serialize_all(dataset, file, map_to_shared_ptr);
}

}  // namespace uff
