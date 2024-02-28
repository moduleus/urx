#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::writer {

template <typename T, typename U, ContainerType = TypeContainer<T>::VALUE>
struct SerializeHdf5;
template <typename T, typename U>
struct SerializeAll;

template <typename T, typename U>
struct SerializeHdf5<T, U, ContainerType::RAW> {
  static void
  f(const std::string& name, const T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Number
    if constexpr (std::is_arithmetic_v<T>) {
      const H5::StrType datatype(*getStdToHdf5().at(nameTypeid<T>()));
      const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
        attribute.write(datatype, &field);
      } else {
        const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
        dataset.write(&field, datatype, dataspace);
      }
    }
    // Enum
    else if constexpr (std::is_enum_v<T>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      const H5::DataSpace dataspace(H5S_SCALAR);
      const std::string value = urx::utils::io::enums::enumToString(field);
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
        attribute.write(datatype, value);
      } else {
        const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
        dataset.write(value, datatype, dataspace);
      }
    }
    // Default
    else {
      const H5::Group group_child(group.createGroup(name));
      SerializeAll<T, U>::f(field, group_child, map, data_field);
    }
  }
};

template <typename T, typename U>
struct SerializeHdf5<T, U, ContainerType::SHARED_PTR> {
  static void
  f(const std::string& name, const T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    SerializeHdf5<typename T::element_type, U>::f(name, *field, group, map, data_field);
  }
};

template <typename T, typename U>
struct SerializeHdf5<T, U, ContainerType::WEAK_PTR> {
  static void
  f(const std::string& name, const T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Never assigned
    if (!field.owner_before(std::weak_ptr<typename T::element_type>{}) &&
        !std::weak_ptr<typename T::element_type>{}.owner_before(field)) {
      return;
    }

    if (auto shared = field.lock()) {
      const std::vector<std::shared_ptr<typename T::element_type>>& all_shared =
          *reinterpret_cast<const std::vector<std::shared_ptr<typename T::element_type>>*>(
              map.at(nameTypeid<typename T::element_type>()));
      auto idx = std::find_if(all_shared.begin(), all_shared.end(),
                              [&shared](const std::shared_ptr<typename T::element_type>& data) {
                                return shared.get() == data.get();
                              });
      if (idx == all_shared.end()) {
        throw std::runtime_error(("Failed to read data from " + name).c_str());
      }
      SerializeHdf5<std::size_t, U>::f(name, std::distance(all_shared.begin(), idx), group, map,
                                       data_field);
    } else {
      throw std::runtime_error(("Failed to read data from " + name).c_str());
    }
  }
};

template <typename T, typename U>
struct SerializeHdf5<T, U, ContainerType::VECTOR> {
  static void
  f(const std::string& name, const T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    if constexpr (std::is_arithmetic_v<typename T::value_type>) {
      const size_t size = field.size();
      const hsize_t dims[1] = {size};
      const H5::DataSpace dataspace = H5::DataSpace(1, dims);
      const H5::PredType* datatype = getStdToHdf5().at(nameTypeid<typename T::value_type>());
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute = group.createAttribute(name, *datatype, dataspace);
        if (size != 0) {
          attribute.write(*datatype, field.data());
        }
      } else {
        const H5::DSetCreatPropList plist;
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR >= 14
        plist.setLayout(size < 8192 ? H5D_COMPACT : H5D_CONTIGUOUS);
#endif
        const H5::DataSet dataset = group.createDataSet(name, *datatype, dataspace, plist);
        dataset.write(field.data(), *datatype);
      }
    } else {
      const H5::Group group_child(group.createGroup(name));

      size_t i = 0;
      for (const auto& iter : field) {
        SerializeHdf5<typename T::value_type, U>::f(
            common::formatIndexWithLeadingZeros(i, ITER_LENGTH), iter, group_child, map,
            data_field);
        i++;
      }
    }
  }
};

template <typename U>
struct SerializeHdf5<std::string, U, ContainerType::RAW> {
  static void f(
      const std::string& name, const std::string& field, const H5::Group& group, MapToSharedPtr&,
      const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>&) {
    const H5::StrType datatype(0, H5T_VARIABLE);
    const H5::DataSpace dataspace(H5S_SCALAR);
    if constexpr (USE_ATTRIBUTE) {
      const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
      attribute.write(datatype, field);
    } else {
      const H5::DSetCreatPropList plist;
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR >= 14
      plist.setLayout(field.size() < 65536 ? H5D_COMPACT : H5D_CONTIGUOUS);
#endif
      const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace, plist);
      dataset.write(field, datatype, dataspace);
    }
  }
};

template <typename U>
struct SerializeHdf5<DoubleNan, U, ContainerType::RAW> {
  static void
  f(const std::string& name, const DoubleNan& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    SerializeHdf5<double, U>::f(name, field.value, group, map, data_field);
  }
};

template <typename U>
struct SerializeHdf5<std::shared_ptr<RawData>, U, ContainerType::SHARED_PTR> {
  static void f(
      const std::string& name, const std::shared_ptr<RawData>& field, const H5::Group& group,
      MapToSharedPtr&,
      const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>&) {
    enum class Format { ARRAY_2D, COMPOUND };
    constexpr Format format = Format::ARRAY_2D;

    const std::unordered_map<DataType, std::type_index> group_dt_to_typeid{
        {DataType::INT16, nameTypeid<int16_t>()},
        {DataType::INT32, nameTypeid<int32_t>()},
        {DataType::FLOAT, nameTypeid<float>()},
        {DataType::DOUBLE, nameTypeid<double>()}};
    const H5::PredType* datatype = getStdToHdf5().at(group_dt_to_typeid.at(field->getDataType()));

    const bool is_complex = field->getSamplingType() == SamplingType::IQ;
    if constexpr (format == Format::ARRAY_2D) {
      const hsize_t dims[2] = {field->getSize(), is_complex ? 2ULL : 1ULL};
      const H5::DataSpace dataspace = H5::DataSpace(2, dims);
      const H5::DataSet dataset = group.createDataSet(name, *datatype, dataspace);
      dataset.write(field->getBuffer(), *datatype);
    } else {
      const std::unordered_map<DataType, size_t> group_dt_to_sizeof{
          {DataType::INT16, sizeof(int16_t)},
          {DataType::INT32, sizeof(int32_t)},
          {DataType::FLOAT, sizeof(float)},
          {DataType::DOUBLE, sizeof(double)}};

      const hsize_t dims[1] = {field->getSize()};
      const H5::DataSpace dataspace = H5::DataSpace(1, dims);

      const size_t size_of = group_dt_to_sizeof.at(field->getDataType());

      const H5::CompType complex_type(size_of * (is_complex ? 2 : 1));

      if (is_complex) {
        complex_type.insertMember("real", 0ULL, *datatype);
        complex_type.insertMember("imag", size_of, *datatype);
      } else {
        complex_type.insertMember("real", 0ULL, *datatype);
      }
      const H5::DataSet dataset = group.createDataSet(name, complex_type, dataspace);
      dataset.write(field->getBuffer(), complex_type);
    }
  }
};

template <typename T, typename U>
struct SerializeAll {
  static void
  f(const T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Need to update map for Probe.
    if constexpr (std::is_same_v<T, Probe>) {
      map.insert({nameTypeid<ElementGeometry>(), &field.element_geometries});
      map.insert({nameTypeid<ImpulseResponse>(), &field.impulse_responses});
    }
    for (const auto& kv : data_field.at(nameTypeid<T>())) {
      std::visit(
          [name = kv.second, field_ptr = &field, &group, &map, &data_field](const auto* var) {
            SerializeHdf5<std::remove_cv_t<std::remove_pointer_t<decltype(var)>>, U>::f(
                name,
                *reinterpret_cast<decltype(var)>(reinterpret_cast<std::uintptr_t>(field_ptr) +
                                                 reinterpret_cast<std::uintptr_t>(var)),
                group, map, data_field);
          },
          std::get<0>(kv));
    }

    if constexpr (std::is_same_v<T, Probe>) {
      map.erase(nameTypeid<ElementGeometry>());
      map.erase(nameTypeid<ImpulseResponse>());
    }
  }
};

}  // namespace urx::utils::io::writer
