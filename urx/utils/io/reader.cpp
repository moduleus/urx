#include <complex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <version>

#include <H5Cpp.h>
#include <boost/pfr.hpp>

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
#include <urx/utils/common.h>
#include <urx/utils/io/enums.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::reader {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

template <typename T>
const std::vector<std::shared_ptr<T>>& getSharedPtr(MapToSharedPtr& map) {
  return *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(
      map.at(std::type_index{typeid(T)}));
}

std::unordered_map<std::type_index, const H5::PredType*> std_to_h5;

constexpr int ITER_LENGTH = 8;

template <typename T>
void deserializeAll(T& field, const H5::Group& group, MapToSharedPtr& map);
template <>
void deserializeAll(std::shared_ptr<RawData>& field, const H5::Group& group, MapToSharedPtr& map);

template <class T>
void deserializeHdf5(const std::string& name, T& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  // Number
  if constexpr (std::is_arithmetic_v<T>) {
    const H5::StrType datatype(*std_to_h5.at(typeid(T)));
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
    deserializeAll(field, group_child, map);
  }
}

// String
template <>
void deserializeHdf5(const std::string& name, std::string& field, const H5::Group& group,
                     MapToSharedPtr&) {
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

// DoubleNan
template <>
void deserializeHdf5(const std::string& name, DoubleNan& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  deserializeHdf5(name, field.value, group, map);
}

// shared_ptr
template <typename T>
void deserializeHdf5(const std::string& name, std::shared_ptr<T>& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  field = std::make_shared<T>();
  deserializeHdf5(name, *field, group, map);
}

// RawData
template <>
void deserializeHdf5(const std::string&, std::shared_ptr<RawData>& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  deserializeAll(field, group, map);
}

// weak_ptr
template <typename T>
void deserializeHdf5(const std::string& name, std::weak_ptr<T>& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  if (group.nameExists(name) || group.attrExists(name)) {
    std::size_t idx;

    deserializeHdf5(name, idx, group, map);

    field = getSharedPtr<T>(map)[idx];
  }
}

// Vector
template <typename T>
void deserializeHdf5(const std::string& name, std::vector<T>& field, const H5::Group& group,
                     MapToSharedPtr& map) {
  if constexpr (std::is_arithmetic_v<T>) {
    const H5::StrType datatype(*std_to_h5.at(typeid(T)));

    H5::DataSet dataset;
    H5::DataSpace dataspace;
    H5::Attribute attribute;

    if (group.nameExists(name)) {
      dataset = group.openDataSet(name);
      dataspace = dataset.getSpace();
    } else {
      attribute = group.openAttribute(name);
      dataspace = attribute.getSpace();
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
  } else {
    const H5::Group group_child(group.openGroup(name));

    size_t i = 0;
    while (group_child.nameExists(common::formatIndexWithLeadingZeros(i, ITER_LENGTH)) ||
           group_child.attrExists(common::formatIndexWithLeadingZeros(i, ITER_LENGTH))) {
      field.push_back(T{});
      deserializeHdf5(common::formatIndexWithLeadingZeros(i, ITER_LENGTH), field.back(),
                      group_child, map);
      i++;
    }
  }
}

template <typename T>
void deserializeAll(T& field, const H5::Group& group, MapToSharedPtr& map) {
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

  boost::pfr::for_each_field(field, [&group, &map, &field](auto& child) {
    deserializeHdf5(SerializeHelper::member_name.at(typeid(T)).at(DIFF_PTR(field, child)), child,
                    group, map);
  });
}

// Need to update map for Probe.
template <>
void deserializeAll(Probe& field, const H5::Group& group, MapToSharedPtr& map) {
  map.insert({typeid(ElementGeometry), &field.element_geometries});
  map.insert({typeid(ImpulseResponse), &field.impulse_responses});
  boost::pfr::for_each_field(field, [&group, &map, &field](auto& child) {
    deserializeHdf5(SerializeHelper::member_name.at(typeid(Probe)).at(DIFF_PTR(field, child)),
                    child, group, map);
  });
  map.erase(typeid(ElementGeometry));
  map.erase(typeid(ImpulseResponse));
}

// for_each_field doesn't support RawData.
template <>
void deserializeAll(std::shared_ptr<RawData>& field, const H5::Group& group, MapToSharedPtr&) {
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

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  // OK
  auto dataset = std::make_shared<Dataset>();

  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);
  MapToSharedPtr map_to_shared_ptr{{typeid(Group), &dataset->acquisition.groups},
                                   {typeid(Probe), &dataset->acquisition.probes},
                                   {typeid(Excitation), &dataset->acquisition.excitations},
                                   {typeid(GroupData), &dataset->acquisition.groups_data}};

  deserializeHdf5("dataset", *dataset, file, map_to_shared_ptr);

  return dataset;
}

}  // namespace urx::utils::io::reader
