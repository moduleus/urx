#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
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

#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transmit_setup.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer_options.h>
#include <urx/utils/serialize_helper.h>

// NOLINTNEXTLINE(bugprone-macro-parentheses)
#define INDEXOF(c, member) (&reinterpret_cast<c*>(0)->member)

namespace urx::utils::io {

namespace details {

inline H5::Group createOrOpenGroup(const H5::Group& group, const std::string& name) {
  if (group.nameExists(name, H5P_DEFAULT)) {
    return group.openGroup(name);
  }

  return group.createGroup(name);
}

inline H5::Attribute createOrOverrideAttribute(const H5::Group& group, const std::string& name,
                                               const H5::StrType& data_type,
                                               const H5::DataSpace& data_space) {
  if (group.attrExists(name)) {
    group.removeAttr(name);
  }

  if (group.nameExists(name)) {
    group.unlink(name);
  }

  return group.createAttribute(name, data_type, data_space);
}

inline H5::DataSet createOrOverrideDataSet(
    const H5::Group& group, const std::string& name, const H5::StrType& data_type,
    const H5::DataSpace& data_space,
    const H5::DSetCreatPropList& create_plist = H5::DSetCreatPropList::DEFAULT) {
  if (group.attrExists(name)) {
    group.removeAttr(name);
  }

  if (group.nameExists(name)) {
    group.unlink(name);
  }

  return group.createDataSet(name, data_type, data_space, create_plist);
}

}  // namespace details

template <typename Dataset, typename AllTypeInVariant, typename Derived>
class WriterBase {
 public:
  WriterBase() {
    if constexpr (std::is_same_v<Dataset, urx::Dataset>) {
      _data_field = urx::utils::getMemberMap();
    }
  }

  void init(const Dataset& dataset) {
    _map_to_shared_ptr[urx::utils::nameTypeid<Group>()] = &dataset.acquisition.groups;
    _map_to_shared_ptr[urx::utils::nameTypeid<Probe>()] = &dataset.acquisition.probes;
    _map_to_shared_ptr[urx::utils::nameTypeid<Excitation>()] = &dataset.acquisition.excitations;
    _map_to_shared_ptr[urx::utils::nameTypeid<GroupData>()] = &dataset.acquisition.groups_data;
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::RAW> serializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    // Number
    if constexpr (std::is_arithmetic_v<T>) {
      const H5::StrType datatype(*getStdToHdf5().at(urx::utils::nameTypeid<T>()));
      const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute =
            details::createOrOverrideAttribute(group, name, datatype, dataspace);
        attribute.write(datatype, &field);
      } else {
        H5::DataSet dataset = details::createOrOverrideDataSet(group, name, datatype, dataspace);
        dataset.write(&field, datatype, dataspace);
        dataset.close();
      }
    }
    // Enum
    else if constexpr (std::is_enum_v<T>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      const H5::DataSpace dataspace(H5S_SCALAR);
      const std::string value = urx::utils::io::enums::enumToString(field);
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute =
            details::createOrOverrideAttribute(group, name, datatype, dataspace);
        attribute.write(datatype, value);
      } else {
        H5::DataSet dataset = details::createOrOverrideDataSet(group, name, datatype, dataspace);
        dataset.write(value, datatype, dataspace);
        dataset.close();
      }
    } else if constexpr (std::is_same_v<T, std::string>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      const H5::DataSpace dataspace(H5S_SCALAR);
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute =
            details::createOrOverrideAttribute(group, name, datatype, dataspace);
        attribute.write(datatype, field);
      } else {
        const H5::DSetCreatPropList plist;
        H5::DataSet dataset =
            details::createOrOverrideDataSet(group, name, datatype, dataspace, plist);
        dataset.write(field, datatype, dataspace);
        dataset.close();
      }
    } else if constexpr (std::is_same_v<T, DoubleNan>) {
      static_cast<Derived*>(this)->template serializeHdf5<double>(name, field.value, group);
    }
    // Default
    else {
      const H5::Group group_child = details::createOrOpenGroup(group, name);

      static_cast<Derived*>(this)->template serializeAll<T>(field, group_child);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::SHARED_PTR> serializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, std::shared_ptr<RawData>>) {
      if (!field) {
        return;
      }

      // getBuffer is null in stream mode.
      if (field->getBuffer() == nullptr && group.nameExists(name)) {
        return;
      }

      const std::unordered_map<DataType, std::type_index> group_dt_to_typeid{
          {DataType::INT16, urx::utils::nameTypeid<int16_t>()},
          {DataType::INT32, urx::utils::nameTypeid<int32_t>()},
          {DataType::FLOAT, urx::utils::nameTypeid<float>()},
          {DataType::DOUBLE, urx::utils::nameTypeid<double>()}};
      const H5::PredType* datatype = getStdToHdf5().at(group_dt_to_typeid.at(field->getDataType()));

      const bool is_complex = field->getSamplingType() == SamplingType::IQ;

      H5::DataSet dataset;
      H5::DataSpace dataspace;
      const hsize_t dims[2] = {field->getSize(), is_complex ? 2ULL : 1ULL};
      const hsize_t maxdims[2] = {H5S_UNLIMITED, is_complex ? 2ULL : 1ULL};
#ifdef WITH_MATLAB
      // There is a conflict between URX and MATLAB when using HDF5.
      // H5::DSetCreatPropList prop;
      // is buggy.
      // So a garbage DataSet is created and its PropList duplicated.
      if (_options.getChunkGroupData() && field->getSize() != 0) {
        dataspace = H5::DataSpace(2, dims);
        dataset = details::createOrOverrideDataSet(group, name, *datatype, dataspace);

        const H5::DataSpace dataspace2 = H5::DataSpace(2, dims, maxdims);
        const H5::DSetCreatPropList prop = dataset.getCreatePlist();
        prop.setChunk(2, dims);

        dataset.close();
        group.unlink(name);

        dataset = details::createOrOverrideDataSet(group, name, *datatype, dataspace2, prop);
      } else {
        dataspace = H5::DataSpace(2, dims);
        dataset = details::createOrOverrideDataSet(group, name, *datatype, dataspace);
      }
#else
      const H5::DSetCreatPropList prop;
      if (_options.getChunkGroupData() && field->getSize() != 0) {
        dataspace = H5::DataSpace(2, dims, maxdims);
        prop.setChunk(2, dims);
      } else {
        dataspace = H5::DataSpace(2, dims);
      }
      dataset = details::createOrOverrideDataSet(group, name, *datatype, dataspace, prop);
#endif

      if (field->getSize() != 0 && field->getBuffer() != nullptr) {
        dataset.write(field->getBuffer(), *datatype);
      }
      dataset.close();
    } else {
      static_cast<Derived*>(this)->template serializeHdf5<typename T::element_type>(name, *field,
                                                                                    group);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::WEAK_PTR> serializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    // Never assigned
    if (!field.owner_before(std::weak_ptr<typename T::element_type>{}) &&
        !std::weak_ptr<typename T::element_type>{}.owner_before(field)) {
      return;
    }

    if (auto shared = field.lock()) {
      const auto& all_shared =
          urx::utils::getSharedPtr<typename T::element_type>(_map_to_shared_ptr);
      auto idx = std::find_if(all_shared.begin(), all_shared.end(),
                              [&shared](const std::shared_ptr<typename T::element_type>& data) {
                                return shared.get() == data.get();
                              });
      if (idx == all_shared.end()) {
        throw std::runtime_error("Failed to found shared pointer assigned to weak pointer " +
                                 group.getObjName() + "/" + name);
      }
      static_cast<Derived*>(this)->template serializeHdf5<std::size_t>(
          name, std::distance(all_shared.begin(), idx), group);
    } else {
      throw std::runtime_error("Invalid weak field from " + group.getObjName() + "/" + name);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::OPTIONAL> serializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    if (!field) {
      return;
    }
    static_cast<Derived*>(this)->template serializeHdf5<typename T::value_type>(name, *field,
                                                                                group);
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::VECTOR> serializeHdf5(
      const std::string& name, const T& field, const H5::Group& group) {
    const size_t size = field.size();
    if constexpr (std::is_arithmetic_v<typename T::value_type>) {
      const hsize_t dims[1] = {size};
      const H5::DataSpace dataspace = H5::DataSpace(1, dims);
      const H5::PredType* datatype =
          getStdToHdf5().at(urx::utils::nameTypeid<typename T::value_type>());
      if constexpr (USE_ATTRIBUTE) {
        const H5::Attribute attribute =
            details::createOrOverrideAttribute(group, name, *datatype, dataspace);
        if (size != 0) {
          attribute.write(*datatype, field.data());
        }
      } else {
        const H5::DSetCreatPropList plist;
        H5::DataSet dataset =
            details::createOrOverrideDataSet(group, name, *datatype, dataspace, plist);
        if (size != 0) {
          dataset.write(field.data(), *datatype);
        }
        dataset.close();
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
        const H5::Attribute attribute =
            details::createOrOverrideAttribute(group, name, datatype, dataspace);
        if (size != 0) {
          attribute.write(datatype, reinterpret_cast<const void*>(c_strings.data()));
        }
      } else {
        const H5::DSetCreatPropList plist;
        H5::DataSet dataset =
            details::createOrOverrideDataSet(group, name, datatype, dataspace, plist);
        if (size != 0) {
          dataset.write(reinterpret_cast<const void*>(c_strings.data()), datatype);
        }
        dataset.close();
      }
    } else {
      const H5::Group group_child = details::createOrOpenGroup(group, name);

      size_t i = 0;
      for (const auto& iter : field) {
        static_cast<Derived*>(this)->template serializeHdf5<typename T::value_type>(
            common::formatIndexWithLeadingZeros(i, ITER_LENGTH), iter, group_child);
        i++;
      }

      while (true) {
        const std::string name_i = common::formatIndexWithLeadingZeros(i, ITER_LENGTH);
        if (group_child.nameExists(name_i)) {
          group_child.unlink(name_i);
        } else if (group_child.attrExists(name_i)) {
          group_child.removeAttr(name_i);
        } else {
          break;
        }
        i++;
      }
    }
  }

  template <typename T>
  void serializeAll(const T& field, const H5::Group& group) {
    // Need to update map for Probe.
    if constexpr (std::is_same_v<T, Probe>) {
      _map_to_shared_ptr.insert(
          {urx::utils::nameTypeid<ElementGeometry>(), &field.element_geometries});
      _map_to_shared_ptr.insert(
          {urx::utils::nameTypeid<ImpulseResponse>(), &field.impulse_responses});
    }

    for (const auto& kv : _data_field.at(urx::utils::nameTypeid<T>())) {
      bool fake_data = false;
      using ExcitationT =
          typename decltype(Dataset::acquisition.excitations)::value_type::element_type;
      void* kv_ptr = std::visit([](auto ptr) { return static_cast<void*>(ptr); }, kv.first);
      if constexpr (std::is_base_of_v<ReceiveSetup, T>) {
        if (_options.getCleanUnusableData() && field.probe.expired() &&
            (kv_ptr == INDEXOF(T, probe_transform) || kv_ptr == INDEXOF(T, active_elements))) {
          fake_data = true;
        }
      }
      if constexpr (std::is_base_of_v<urx::detail::TransmitSetup<ExcitationT>, T>) {
        if (_options.getCleanUnusableData() && field.probe.expired() &&
            (kv_ptr == INDEXOF(T, probe_transform) || kv_ptr == INDEXOF(T, active_elements) ||
             kv_ptr == INDEXOF(T, excitations) || kv_ptr == INDEXOF(T, delays))) {
          fake_data = true;
        }
      }
      std::visit(
          [this, name = kv.second, field_ptr = &field, &group, &fake_data](const auto* var) {
            const std::remove_cv_t<std::remove_pointer_t<decltype(var)>> fake_field = {};
            static_cast<Derived*>(this)
                ->template serializeHdf5<std::remove_const_t<decltype(fake_field)>>(
                    name,
                    fake_data ? fake_field
                              : *reinterpret_cast<decltype(var)>(
                                    reinterpret_cast<std::uintptr_t>(field_ptr) +
                                    reinterpret_cast<std::uintptr_t>(var)),
                    group);
          },
          kv.first);
    }

    if constexpr (std::is_same_v<T, Probe>) {
      _map_to_shared_ptr.erase(urx::utils::nameTypeid<ElementGeometry>());
      _map_to_shared_ptr.erase(urx::utils::nameTypeid<ImpulseResponse>());
    }
  }

  const WriterOptions& getOptions() const { return _options; }
  WriterOptions& getOptions() { return _options; }
  void setOptions(const WriterOptions& options) { _options = options; }

 protected:
  urx::utils::MapToSharedPtr _map_to_shared_ptr;
  std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>
      _data_field;

 private:
  WriterOptions _options;
};

template <typename Dataset, typename AllTypeInVariant,
          template <typename, typename, typename...> class Base>
class WriterDatasetBase
    : public Base<Dataset, AllTypeInVariant, WriterDatasetBase<Dataset, AllTypeInVariant, Base>> {
 public:
  WriterDatasetBase()
      : Base<Dataset, AllTypeInVariant, WriterDatasetBase<Dataset, AllTypeInVariant, Base>>() {}

  void write(const H5::H5File& h5_file, const Dataset& dataset) {
    try {
      this->init(dataset);

      this->serializeHdf5("dataset", dataset, h5_file);
    } catch (const H5::FileIException& e) {
      throw WriteFileException("Failed to write dataset to file.\n" + e.getFuncName() + "\n" +
                               e.getDetailMsg());
    } catch (const H5::DataSetIException& e) {
      throw WriteFileException("Failed to write dataset to file.\n" + e.getFuncName() + "\n" +
                               e.getDetailMsg());
    }
  }

  void write(const std::string& filename, const Dataset& dataset) {
    try {
      H5::H5File file;
      if (std::filesystem::exists(filename)) {
        file = H5::H5File(filename.c_str(), H5F_ACC_RDWR);
      } else {
        file = H5::H5File(filename.c_str(), H5F_ACC_TRUNC);
      }

      write(file, dataset);
    } catch (const H5::FileIException& e) {
      throw WriteFileException("Failed to write " + filename + ".\n" + e.getFuncName() + "\n" +
                               e.getDetailMsg());
    } catch (const H5::DataSetIException& e) {
      throw WriteFileException("Failed to write " + filename + ".\n" + e.getFuncName() + "\n" +
                               e.getDetailMsg());
    }
  }
};

using WriterDataset = urx::utils::io::WriterDatasetBase<Dataset, urx::utils::AllTypeInVariant,
                                                        urx::utils::io::WriterBase>;

}  // namespace urx::utils::io

#undef INDEXOF
