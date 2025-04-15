#pragma once

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
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io {

template <typename Dataset, typename AllTypeInVariant, typename Derived>
class WriterBase {
 public:
  WriterBase(
      const std::string& filename, const Dataset& dataset,
      const std::unordered_map<std::type_index,
                               std::vector<std::pair<AllTypeInVariant, std::string>>>& data_field)
      : _filename(filename), _dataset(dataset), _data_field(data_field) {
    if constexpr (std::is_same_v<Dataset, urx::Dataset>) {
      _map_to_shared_ptr = {{nameTypeid<Group>(), &dataset.acquisition.groups},
                            {nameTypeid<Probe>(), &dataset.acquisition.probes},
                            {nameTypeid<Excitation>(), &dataset.acquisition.excitations},
                            {nameTypeid<GroupData>(), &dataset.acquisition.groups_data}};
    }
  }

  void write() {
    try {
      const H5::H5File file(_filename.data(), H5F_ACC_TRUNC);

      static_cast<Derived*>(this)->template SerializeHdf5<Dataset>("dataset", _dataset, file);
    } catch (const H5::FileIException&) {
      throw WriteFileException("Failed to write " + _filename + ".");
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::RAW> SerializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
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
      static_cast<Derived*>(this)->template SerializeAll<T>(field, group_child);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::SHARED_PTR> SerializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    static_cast<Derived*>(this)->template SerializeHdf5<typename T::element_type>(name, *field,
                                                                                  group);
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::WEAK_PTR> SerializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    // Never assigned
    if (!field.owner_before(std::weak_ptr<typename T::element_type>{}) &&
        !std::weak_ptr<typename T::element_type>{}.owner_before(field)) {
      return;
    }

    if (auto shared = field.lock()) {
      const std::vector<std::shared_ptr<typename T::element_type>>& all_shared =
          *reinterpret_cast<const std::vector<std::shared_ptr<typename T::element_type>>*>(
              _map_to_shared_ptr.at(nameTypeid<typename T::element_type>()));
      auto idx = std::find_if(all_shared.begin(), all_shared.end(),
                              [&shared](const std::shared_ptr<typename T::element_type>& data) {
                                return shared.get() == data.get();
                              });
      if (idx == all_shared.end()) {
        throw std::runtime_error(("Failed to found shared pointer assigned to weak pointer " +
                                  group.getObjName() + "/" + name)
                                     .c_str());
      }
      static_cast<Derived*>(this)->template SerializeHdf5<std::size_t>(
          name, std::distance(all_shared.begin(), idx), group);
    } else {
      throw std::runtime_error(
          ("Invalid weak field from " + group.getObjName() + "/" + name).c_str());
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::OPTIONAL> SerializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    if (!field) {
      return;
    }
    static_cast<Derived*>(this)->template SerializeHdf5<typename T::value_type>(name, *field,
                                                                                group);
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::VECTOR> SerializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    const size_t size = field.size();
    if (size == 0) {
      if constexpr (std::is_arithmetic_v<typename T::value_type>) {
        const hsize_t dims[1] = {size};
        const H5::DataSpace dataspace = H5::DataSpace(1, dims);
        const H5::PredType* datatype = getStdToHdf5().at(nameTypeid<typename T::value_type>());
        const H5::DSetCreatPropList plist;
        group.createDataSet(name, *datatype, dataspace, plist);
      }
      return;
    }
    if constexpr (std::is_arithmetic_v<typename T::value_type>) {
      const hsize_t dims[1] = {size};
      const H5::DataSpace dataspace = H5::DataSpace(1, dims);
      const H5::PredType* datatype = getStdToHdf5().at(nameTypeid<typename T::value_type>());
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute = group.createAttribute(name, *datatype, dataspace);
        attribute.write(*datatype, field.data());
      } else {
        const H5::DSetCreatPropList plist;
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR >= 14
        plist.setLayout(size < 8192 ? H5D_COMPACT : H5D_CONTIGUOUS);
#endif
        const H5::DataSet dataset = group.createDataSet(name, *datatype, dataspace, plist);
        dataset.write(field.data(), *datatype);
      }
    } else if constexpr (std::is_same_v<typename T::value_type, std::string>) {
      const hsize_t dims[1] = {size};
      const H5::DataSpace dataspace = H5::DataSpace(1, dims);
      const H5::StrType datatype(0, H5T_VARIABLE);

      std::vector<const char*> c_strings;
      c_strings.reserve(size);
      for (const auto& str : field) {
        c_strings.push_back(str.c_str());
      }

      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute = group.createAttribute(name, datatype, dataspace);
        attribute.write(datatype, c_strings.data());
      } else {
        const H5::DSetCreatPropList plist;
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR >= 14
        plist.setLayout(size < 8192 ? H5D_COMPACT : H5D_CONTIGUOUS);
#endif
        const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace, plist);

        dataset.write(c_strings.data(), datatype);
      }
    } else {
      const H5::Group group_child(group.createGroup(name));

      size_t i = 0;
      for (const auto& iter : field) {
        static_cast<Derived*>(this)->template SerializeHdf5<typename T::value_type>(
            common::formatIndexWithLeadingZeros(i, ITER_LENGTH), iter, group_child);
        i++;
      }
    }
  }

  template <typename T>
  typename std::enable_if_t<std::is_same_v<T, std::string> &&
                            TypeContainer<T>::VALUE == ContainerType::RAW>
  SerializeHdf5(const std::string& name, const std::string& field, const H5::Group& group) {
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

  template <typename T>
  typename std::enable_if_t<std::is_same_v<T, DoubleNan> &&
                            TypeContainer<T>::VALUE == ContainerType::RAW>
  SerializeHdf5(const std::string& name, const DoubleNan& field, const H5::Group& group) {
    static_cast<Derived*>(this)->template SerializeHdf5<double>(name, field.value, group);
  }

  template <typename T>
  typename std::enable_if_t<std::is_same_v<T, std::shared_ptr<RawData>> &&
                            TypeContainer<T>::VALUE == ContainerType::SHARED_PTR>
  SerializeHdf5(const std::string& name, const std::shared_ptr<RawData>& field,
                const H5::Group& group) {
    if (field->getSize() == 0) {
      return;
    }

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

  template <typename T>
  void SerializeAll(const T& field, const H5::Group& group) {
    // Need to update map for Probe.
    if constexpr (std::is_same_v<T, Probe>) {
      _map_to_shared_ptr.insert({nameTypeid<ElementGeometry>(), &field.element_geometries});
      _map_to_shared_ptr.insert({nameTypeid<ImpulseResponse>(), &field.impulse_responses});
    }
    for (const auto& kv : _data_field.at(nameTypeid<T>())) {
      std::visit(
          [this, name = kv.second, field_ptr = &field, &group](const auto* var) {
            static_cast<Derived*>(this)
                ->template SerializeHdf5<std::remove_cv_t<std::remove_pointer_t<decltype(var)>>>(
                    name,
                    *reinterpret_cast<decltype(var)>(reinterpret_cast<std::uintptr_t>(field_ptr) +
                                                     reinterpret_cast<std::uintptr_t>(var)),
                    group);
          },
          std::get<0>(kv));
    }

    if constexpr (std::is_same_v<T, Probe>) {
      _map_to_shared_ptr.erase(nameTypeid<ElementGeometry>());
      _map_to_shared_ptr.erase(nameTypeid<ImpulseResponse>());
    }
  }

 private:
  const std::string& _filename;
  const Dataset& _dataset;
  const std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>&
      _data_field;
  MapToSharedPtr _map_to_shared_ptr;
};

template <typename Dataset, typename AllTypeInVariant>
class Writer : public WriterBase<Dataset, AllTypeInVariant, Writer<Dataset, AllTypeInVariant>> {
 public:
  Writer(const std::string& filename, const Dataset& dataset,
         const std::unordered_map<
             std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>& data_field)
      : WriterBase<Dataset, AllTypeInVariant, Writer<Dataset, AllTypeInVariant>>(filename, dataset,
                                                                                 data_field) {}
};

}  // namespace urx::utils::io
