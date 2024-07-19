#pragma once

#include <algorithm>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include <version>

#include <H5Cpp.h>

#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::reader {

template <typename T, typename U, ContainerType = TypeContainer<T>::VALUE>
struct DeserializeHdf5;
template <typename T, typename U>
struct DeserializeAll;

template <typename T, typename U>
struct DeserializeHdf5<T, U, ContainerType::RAW> {
  static void
  f(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Number
    if constexpr (std::is_arithmetic_v<T>) {
      const H5::StrType datatype(*getStdToHdf5().at(nameTypeid<T>()));
      if (group.nameExists(name)) {
        const H5::DataSet dataset = group.openDataSet(name);
        dataset.read(&field, datatype);
      } else {
        const H5::Attribute attribute = group.openAttribute(name);
        attribute.read(datatype, &field);
      }
    }
    // Enum
    else if constexpr (std::is_enum_v<T>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      const H5::DataSpace dataspace(H5S_SCALAR);
      std::string value;
      if (group.nameExists(name)) {
        const H5::DataSet dataset = group.openDataSet(name);
        dataset.read(value, datatype, dataspace);
      } else {
        const H5::Attribute attribute = group.openAttribute(name);
        attribute.read(datatype, value);
      }

      field = urx::utils::io::enums::stringToEnum<T>(value);
    }
    // Default
    else {
      const H5::Group group_child(group.openGroup(name));
      DeserializeAll<T, U>::f(field, group_child, map, data_field);
    }
  }
};

template <typename T, typename U>
struct DeserializeHdf5<T, U, ContainerType::SHARED_PTR> {
  static void
  f(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    field = std::make_shared<typename T::element_type>();
    DeserializeHdf5<typename T::element_type, U>::f(name, *field, group, map, data_field);
  }
};

template <typename T, typename U>
struct DeserializeHdf5<T, U, ContainerType::OPTIONAL> {
  static void
  f(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    if (group.nameExists(name) || group.attrExists(name)) {
      field = typename T::value_type{};
      DeserializeHdf5<typename T::value_type, U>::f(name, *field, group, map, data_field);
    }
  }
};

template <typename T, typename U>
struct DeserializeHdf5<T, U, ContainerType::WEAK_PTR> {
  static void
  f(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    if (group.nameExists(name) || group.attrExists(name)) {
      std::size_t idx;

      DeserializeHdf5<std::size_t, U>::f(name, idx, group, map, data_field);

      field = getSharedPtr<typename T::element_type>(map)[idx];
    }
  }
};

template <typename T, typename U>
struct DeserializeHdf5<T, U, ContainerType::VECTOR> {
  static void
  f(const std::string& name, T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    if constexpr (std::is_arithmetic_v<typename T::value_type>) {
      const H5::StrType datatype(*getStdToHdf5().at(nameTypeid<typename T::value_type>()));

      H5::DataSet dataset;
      H5::DataSpace dataspace;
      H5::Attribute attribute;

      if (group.nameExists(name)) {
        dataset = group.openDataSet(name);
        dataspace = dataset.getSpace();
      } else if (group.attrExists(name)) {
        attribute = group.openAttribute(name);
        dataspace = attribute.getSpace();
      } else {
        return;
      }

      const int ndims = dataspace.getSimpleExtentNdims();
      std::vector<hsize_t> dimension;
      dimension.resize(ndims);
      dataspace.getSimpleExtentDims(dimension.data());
      field.resize(dimension[0]);
      if (dimension[0] != 0) {
        if (group.nameExists(name)) {
          dataset.read(field.data(), datatype);
        } else {
          attribute.read(datatype, field.data());
        }
      }
    } else if constexpr (std::is_same_v<typename T::value_type, std::string>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      H5::DataSet dataset;
      H5::Attribute attribute;
      H5::DataSpace dataspace;

      if (group.nameExists(name)) {
        dataset = group.openDataSet(name);
        dataspace = dataset.getSpace();
      } else if (group.attrExists(name)) {
        attribute = group.openAttribute(name);
        dataspace = attribute.getSpace();
      } else {
        return;
      }

      const int ndims = dataspace.getSimpleExtentNdims();
      std::vector<hsize_t> dimension;
      dimension.resize(ndims);
      dataspace.getSimpleExtentDims(dimension.data());
      field.reserve(dimension[0]);

      std::vector<char*> field_char;
      field_char.resize(dimension[0]);

      if (dimension[0] != 0) {
        if (group.nameExists(name)) {
          dataset.read(field_char.data(), datatype, dataspace);
        } else {
          attribute.read(datatype, field_char.data());
        }
        for (char* str_i : field_char) {
          field.push_back(str_i);
          // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
          delete[] str_i;
        }
      }
    } else {
      if (!group.nameExists(name)) {
        return;
      }

      const H5::Group group_child(group.openGroup(name));

      size_t i = 0;
      while (group_child.nameExists(common::formatIndexWithLeadingZeros(i, ITER_LENGTH)) ||
             group_child.attrExists(common::formatIndexWithLeadingZeros(i, ITER_LENGTH))) {
        field.push_back(typename T::value_type{});
        DeserializeHdf5<typename T::value_type, U>::f(
            common::formatIndexWithLeadingZeros(i, ITER_LENGTH), field.back(), group_child, map,
            data_field);
        i++;
      }
    }
  }
};

template <typename U>
struct DeserializeHdf5<std::string, U, ContainerType::RAW> {
  static void f(
      const std::string& name, std::string& field, const H5::Group& group, MapToSharedPtr&,
      const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>&) {
    const H5::StrType datatype(0, H5T_VARIABLE);
    const H5::DataSpace dataspace(H5S_SCALAR);
    if (group.nameExists(name)) {
      const H5::DataSet dataset = group.openDataSet(name);
      dataset.read(field, datatype, dataspace);
    } else {
      const H5::Attribute attribute = group.openAttribute(name);
      attribute.read(datatype, field);
    }
  }
};

template <typename U>
struct DeserializeHdf5<DoubleNan, U, ContainerType::RAW> {
  static void
  f(const std::string& name, DoubleNan& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    DeserializeHdf5<double, U>::f(name, field.value, group, map, data_field);
  }
};

template <typename U>
struct DeserializeHdf5<std::shared_ptr<RawData>, U, ContainerType::SHARED_PTR> {
  static void
  f(const std::string&, std::shared_ptr<RawData>& field, const H5::Group& group,
    MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    DeserializeAll<std::shared_ptr<RawData>, U>::f(field, group, map, data_field);
  }
};

template <typename T, typename U>
struct DeserializeAll {
  static void
  f(T& field, const H5::Group& group, MapToSharedPtr& map,
    const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>& data_field) {
    // Need to update map for Probe.
    if constexpr (std::is_same_v<T, Probe>) {
      map.insert({nameTypeid<ElementGeometry>(), &field.element_geometries});
      map.insert({nameTypeid<ImpulseResponse>(), &field.impulse_responses});
    }

    for (const auto& kv : data_field.at(nameTypeid<T>())) {
      std::visit(
          [name = kv.second, field_ptr = &field, &group, &map, &data_field](auto* var) {
            DeserializeHdf5<std::remove_pointer_t<decltype(var)>, U>::f(
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

template <typename U>
struct DeserializeAll<std::shared_ptr<RawData>, U> {
  static void f(
      std::shared_ptr<RawData>& field, const H5::Group& group, MapToSharedPtr&,
      const std::unordered_map<std::type_index, std::vector<std::pair<U, std::string>>>&) {
    if (!group.nameExists("raw_data")) {
      return;
    }
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
};

}  // namespace urx::utils::io::reader
