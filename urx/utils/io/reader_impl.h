#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <functional>
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
#include <urx/urx.h>
#include <urx/utils/cpp.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/detail/hdf5.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/raw_data_helper.h>
#include <urx/utils/serialize_helper.h>

namespace urx::utils::io {

struct FindMaxData {
  int max_value = -1;
  std::string group_path;
};

template <typename T, typename Derived>
struct DeserializeData {
  Derived* self;
  T* field_ptr;
  const H5::Group* group_ptr;
  std::string group_path;
};

template <typename Dataset, typename AllTypeInVariant, typename Derived>
class ReaderBase {
 public:
  ReaderBase() {
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
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::RAW> deserializeHdf5(
      const std::string& name, T& field, const H5::Group& group) {
    // Number
    if constexpr (std::is_arithmetic_v<T>) {
      const H5::StrType datatype(*getStdToHdf5().at(urx::utils::nameTypeid<T>()));
      if (group.nameExists(name)) {
        const H5::DataSet dataset = group.openDataSet(name);
        dataset.read(&field, datatype);
      } else if (group.attrExists(name)) {
        const H5::Attribute attribute = group.openAttribute(name);
        attribute.read(datatype, &field);
      } else {
        throw std::runtime_error("Failed to read " + group.getObjName() + "/" + name);
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
      } else if (group.attrExists(name)) {
        const H5::Attribute attribute = group.openAttribute(name);
        attribute.read(datatype, value);
      } else {
        throw std::runtime_error("Failed to read " + group.getObjName() + "/" + name);
      }

      field = enums::stringToEnum<T>(value);
    } else if constexpr (std::is_same_v<T, std::string>) {
      const H5::StrType datatype(0, H5T_VARIABLE);
      const H5::DataSpace dataspace(H5S_SCALAR);
      if (group.nameExists(name)) {
        const H5::DataSet dataset = group.openDataSet(name);
        dataset.read(field, datatype, dataspace);
      } else if (group.attrExists(name)) {
        const H5::Attribute attribute = group.openAttribute(name);
        attribute.read(datatype, field);
      } else {
        throw std::runtime_error("Failed to read " + group.getObjName() + "/" + name);
      }
    } else if constexpr (std::is_same_v<T, DoubleNan>) {
      static_cast<Derived*>(this)->template deserializeHdf5<double>(name, field.value, group);
    }

    // Default
    else {
      if constexpr (std::is_same_v<T, urx::Dataset>) {
        _map_to_shared_ptr[urx::utils::nameTypeid<Group>()] = &field.acquisition.groups;
        _map_to_shared_ptr[urx::utils::nameTypeid<Probe>()] = &field.acquisition.probes;
        _map_to_shared_ptr[urx::utils::nameTypeid<Excitation>()] = &field.acquisition.excitations;
        _map_to_shared_ptr[urx::utils::nameTypeid<GroupData>()] = &field.acquisition.groups_data;
      }
      const H5::Group group_child(group.openGroup(name));
      static_cast<Derived*>(this)->template deserializeAll<T>(field, group_child);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::SHARED_PTR> deserializeHdf5(
      const std::string& name, T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, std::shared_ptr<RawData>>) {
      static_cast<Derived*>(this)->template deserializeAll<std::shared_ptr<RawData>>(field, group);
    } else {
      field = std::make_shared<typename T::element_type>();
      static_cast<Derived*>(this)->template deserializeHdf5<typename T::element_type>(name, *field,
                                                                                      group);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::OPTIONAL> deserializeHdf5(
      const std::string& name, T& field, const H5::Group& group) {
    if (group.nameExists(name) || group.attrExists(name)) {
      field = typename T::value_type{};
      static_cast<Derived*>(this)->template deserializeHdf5<typename T::value_type>(name, *field,
                                                                                    group);
    }
  }

  template <typename T>
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::WEAK_PTR> deserializeHdf5(
      const std::string& name, T& field, const H5::Group& group) {
    if (group.nameExists(name) || group.attrExists(name)) {
      std::size_t idx;

      static_cast<Derived*>(this)->template deserializeHdf5<std::size_t>(name, idx, group);

      const auto& map_i = urx::utils::getSharedPtr<typename T::element_type>(_map_to_shared_ptr);

      if (map_i.size() > idx) {
        field = map_i[idx];
      } else {
        _async_weak_assign.push_back([&field, &map_i, idx]() { field = map_i[idx]; });
      }
    }
  }

  template <typename T>
  // NOLINTNEXTLINE(misc-no-recursion)
  typename std::enable_if_t<TypeContainer<T>::VALUE == ContainerType::VECTOR> deserializeHdf5(
      const std::string& name, T& field, const H5::Group& group) {
    if constexpr (std::is_arithmetic_v<typename T::value_type>) {
      const H5::StrType datatype(
          *getStdToHdf5().at(urx::utils::nameTypeid<typename T::value_type>()));

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
      field.resize(static_cast<size_t>(dimension[0]));
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
      field.reserve(static_cast<size_t>(dimension[0]));

      std::vector<char*> field_char;
      field_char.resize(static_cast<size_t>(dimension[0]));

      if (dimension[0] != 0) {
        if (group.nameExists(name)) {
          dataset.read(reinterpret_cast<void*>(field_char.data()), datatype, dataspace);
        } else {
          attribute.read(datatype, reinterpret_cast<void*>(field_char.data()));
        }
        for (char* str_i : field_char) {
          field.push_back(str_i);
          // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
          free(str_i);
        }
      }
    } else {
      if (!group.nameExists(name, H5P_DEFAULT)) {
        return;
      }

      const H5::Group group_child(group.openGroup(name));

      const hsize_t number_attrs = group_child.getNumAttrs();
      const hsize_t number_dataset = group_child.getNumObjs();

      if (number_attrs != 0 && number_dataset != 0) {
        throw std::runtime_error("Reader doesn't support mixing attributs and dataset in vector " +
                                 group_child.getObjName() + "/" + name);
      }

      FindMaxData data_max;
      data_max.group_path = group.getObjName() + "/" + name;
      if (number_attrs) {
        hsize_t idx = 0;
        H5Aiterate2(
            group_child.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, &idx,
            [](hid_t /*loc_id*/, const char* name_attr, const H5A_info_t* /*ainfo*/,
               void* operator_data) -> herr_t {
              auto* data = static_cast<FindMaxData*>(operator_data);
              try {
                data->max_value = std::max(data->max_value, std::stoi(name_attr));
              } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid name_attr for " + data->group_path + "/" +
                                         name_attr);
              }
              return 0;
            },
            &data_max);
      }
      if (number_dataset) {
        H5Literate(
            group_child.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
            [](hid_t /*loc_id*/, const char* name_dataset, const H5L_info_t* /*info*/,
               void* operator_data) -> herr_t {
              auto* data = static_cast<FindMaxData*>(operator_data);
              try {
                data->max_value = std::max(data->max_value, std::stoi(name_dataset));
              } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid name_dataset for " + data->group_path + "/" +
                                         name_dataset);
              }
              return 0;
            },
            &data_max);
      }

      field.resize(data_max.max_value + 1);

      DeserializeData<T, Derived> data_deserialize{
          static_cast<Derived*>(this), &field, &group_child, group_child.getObjName() + "/" + name};
      if (number_attrs) {
        hsize_t idx = 0;
        H5Aiterate2(
            group_child.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, &idx,
            [](hid_t /*loc_id*/, const char* name_attr, const H5A_info_t* /*ainfo*/,
               void* operator_data) -> herr_t {
              auto* data = static_cast<DeserializeData<T, Derived>*>(operator_data);
              try {
                const int value = std::stoi(name_attr);
                data->self->template deserializeHdf5<typename T::value_type>(
                    name_attr, (*data->field_ptr)[value], *data->group_ptr);
              } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid name_attr for " + data->group_path + "/" +
                                         name_attr);
              }
              return 0;
            },
            &data_deserialize);
      }
      if (number_dataset) {
        H5Literate(
            group_child.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
            [](hid_t /*loc_id*/, const char* name_dataset, const H5L_info_t* /*info*/,
               void* operator_data) -> herr_t {
              auto* data = static_cast<DeserializeData<T, Derived>*>(operator_data);
              try {
                const int value = std::stoi(name_dataset);
                data->self->template deserializeHdf5<typename T::value_type>(
                    name_dataset, (*data->field_ptr)[value], *data->group_ptr);
              } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid name_dataset for " + data->group_path + "/" +
                                         name_dataset);
              }
              return 0;
            },
            &data_deserialize);
      }
    }
  }

  template <typename T>
  void deserializeAll(T& field, const H5::Group& group) {
    if constexpr (std::is_same_v<T, std::shared_ptr<RawData>>) {
      if (!group.nameExists("raw_data")) {
        return;
      }

      const H5::DataSet dataset = group.openDataSet("raw_data");
      const H5::DataSpace dataspace = dataset.getSpace();
      const H5::DataType datatype = dataset.getDataType();

      hsize_t dimension[2];
      dataspace.getSimpleExtentDims(dimension);

      if (_options.getRawDataLoadPolicy() == RawDataLoadPolicy::STREAM) {
        field = urx::utils::rawDataFactory<RawDataStream>(
            urx::utils::io::detail::h5PredTypeToDataType(datatype),
            dimension[1] == 1 ? SamplingType::RF : SamplingType::IQ,
            static_cast<size_t>(dimension[0]));
        return;
      }

      field = urx::utils::rawDataFactory<RawDataNoInit>(
          urx::utils::io::detail::h5PredTypeToDataType(datatype),
          dimension[1] == 1 ? SamplingType::RF : SamplingType::IQ,
          static_cast<size_t>(dimension[0]));

      dataset.read(field->getBuffer(), datatype);
    } else {
      // Need to update map for Probe.
      if constexpr (std::is_same_v<T, Probe>) {
        _map_to_shared_ptr.insert(
            {urx::utils::nameTypeid<ElementGeometry>(), &field.element_geometries});
        _map_to_shared_ptr.insert(
            {urx::utils::nameTypeid<ImpulseResponse>(), &field.impulse_responses});
      }

      for (const auto& kv : _data_field.at(urx::utils::nameTypeid<T>())) {
        std::visit(
            [this, name = kv.second, field_ptr = &field, &group](auto* var) {
              static_cast<Derived*>(this)
                  ->template deserializeHdf5<std::remove_pointer_t<decltype(var)>>(
                      name,
                      *reinterpret_cast<decltype(var)>(reinterpret_cast<std::uintptr_t>(field_ptr) +
                                                       reinterpret_cast<std::uintptr_t>(var)),
                      group);
            },
            std::get<0>(kv));
      }

      if constexpr (std::is_same_v<T, Probe>) {
        _map_to_shared_ptr.erase(urx::utils::nameTypeid<ElementGeometry>());
        _map_to_shared_ptr.erase(urx::utils::nameTypeid<ImpulseResponse>());
      }
    }
  }

  const ReaderOptions& getOptions() const { return _options; }
  ReaderOptions& getOptions() { return _options; }
  void setOptions(const ReaderOptions& options) { _options = options; }

 private:
  ReaderOptions _options;

 protected:
  urx::utils::MapToSharedPtr _map_to_shared_ptr;
  std::vector<std::function<void()>> _async_weak_assign;
  std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>
      _data_field;
};

template <typename Dataset, typename AllTypeInVariant,
          template <typename, typename, typename...> class Base>
class ReaderDatasetBase
    : public Base<Dataset, AllTypeInVariant, ReaderDatasetBase<Dataset, AllTypeInVariant, Base>> {
 public:
  explicit ReaderDatasetBase()
      : Base<Dataset, AllTypeInVariant, ReaderDatasetBase<Dataset, AllTypeInVariant, Base>>() {}

  void read(const H5::H5File& h5_file, Dataset& dataset) {
    this->init(dataset);

    this->template deserializeHdf5<Dataset>("dataset", dataset, h5_file);

    for (auto& funct : this->_async_weak_assign) {
      funct();
    }

    if (dataset.version.major != urx::URX_VERSION_MAJOR) {
      throw ReadFileException("Reader only support urx version " +
                              std::to_string(urx::URX_VERSION_MAJOR) + ". File has version " +
                              std::to_string(dataset.version.major) + ".");
    }

    dataset.version.minor = urx::URX_VERSION_MINOR;
    dataset.version.patch = urx::URX_VERSION_PATCH;
  }

  void read(const std::string& filename, Dataset& dataset) {
    try {
      const H5::H5File file(filename.c_str(), H5F_ACC_RDONLY);

      read(file, dataset);
    } catch (const H5::FileIException&) {
      throw ReadFileException("Failed to read " + filename + ".");
    }
  }
};

using ReaderDataset = urx::utils::io::ReaderDatasetBase<Dataset, urx::utils::AllTypeInVariant,
                                                        urx::utils::io::ReaderBase>;

}  // namespace urx::utils::io
