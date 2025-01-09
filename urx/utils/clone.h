#pragma once

#include <memory>
#include <type_traits>

#include <urx/acquisition.h>
#include <urx/detail/raw_data.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>

template <typename T>
using RawType = T;

namespace urx::utils {

// Get type associated to SamplingType
template <SamplingType enum_value, typename T>
struct SamplingTypeSelector;

template <typename T>
struct SamplingTypeSelector<SamplingType::IQ, T> {
  using Type = std::complex<T>;
};

template <typename T>
struct SamplingTypeSelector<SamplingType::RF, T> {
  using Type = T;
};

template <SamplingType enum_value, typename T>
using SamplingTypeType = typename SamplingTypeSelector<enum_value, T>::type;

// Get type associated to DataType
template <DataType enum_value>
struct DataTypeSelector;

template <>
struct DataTypeSelector<DataType::DOUBLE> {
  using Type = double;
};

template <>
struct DataTypeSelector<DataType::FLOAT> {
  using Type = float;
};

template <>
struct DataTypeSelector<DataType::INT32> {
  using Type = int32_t;
};

template <>
struct DataTypeSelector<DataType::INT16> {
  using Type = int16_t;
};

template <DataType enum_value>
using DataTypeType = typename DataTypeSelector<enum_value>::type;

// Get type associated to SamplingType and DataType
// Waiting C++20 to be used in clone(GroupData)
template <SamplingType sampling_enum_value, DataType data_enum_value>
using SamplingDataType = SamplingTypeType<sampling_enum_value, DataTypeType<data_enum_value>>;

template <typename T, typename U = T>
inline U clone(const T& value) {
  if constexpr (std::is_pointer_v<T>) {
    return value ? new std::remove_pointer_t<T>(clone(*value)) : nullptr;
  } else if constexpr (utils::IsSharedPtr<T>::value) {
    return value ? std::make_shared<typename T::element_type>(clone(*value)) : nullptr;
  } else {
    return value;
  }
}

template <>
inline Probe clone(const Probe& value) {
  Probe probe_cloned(value);

  for (size_t eg_id = 0; eg_id < value.element_geometries.size(); ++eg_id) {
    probe_cloned.element_geometries[eg_id] = clone(value.element_geometries.at(eg_id));
  }
  for (size_t ir_id = 0; ir_id < value.impulse_responses.size(); ++ir_id) {
    probe_cloned.impulse_responses[ir_id] = clone(value.impulse_responses.at(ir_id));
  }

  for (size_t e_id = 0; e_id < value.elements.size(); ++e_id) {
    int32_t const eg_id =
        getEltId(value.element_geometries, value.elements.at(e_id).element_geometry);
    int32_t const ir_id =
        getEltId(value.impulse_responses, value.elements.at(e_id).impulse_response);
    if (eg_id < 0) {
      probe_cloned.elements.at(e_id).element_geometry = std::weak_ptr<ElementGeometry>();
    } else {
      probe_cloned.elements.at(e_id).element_geometry = probe_cloned.element_geometries.at(eg_id);
    }
    if (ir_id < 0) {
      probe_cloned.elements.at(e_id).impulse_response = std::weak_ptr<ImpulseResponse>();
    } else {
      probe_cloned.elements.at(e_id).impulse_response = probe_cloned.impulse_responses.at(ir_id);
    }
  }
  return probe_cloned;
}

template <>
inline std::shared_ptr<Probe> clone(const std::shared_ptr<Probe>& value) {
  return (value == nullptr) ? nullptr : std::make_shared<Probe>(clone(*value));
}

template <typename T>
inline std::shared_ptr<RawDataVector<T>> cloneRawData(const RawDataVector<T>* rd) {
  std::vector<T> const data(static_cast<const T*>(rd->getBuffer()),
                            static_cast<const T*>(rd->getBuffer()) + rd->getSize());

  std::shared_ptr<RawDataVector<T>> rd_cloned = std::make_shared<RawDataVector<T>>(data);

  return rd_cloned;
}

template <typename T>
inline std::shared_ptr<RawDataNoInit<T>> cloneRawData(const RawDataNoInit<T>* rd) {
  std::shared_ptr<RawDataNoInit<T>> rd_cloned = std::make_shared<RawDataNoInit<T>>(rd->getSize());
  std::memcpy(rd_cloned->getBuffer(), rd->getBuffer(), rd->getSize() * sizeof(T));

  return rd_cloned;
}

template <typename T>
inline std::shared_ptr<RawDataWeak<T>> cloneRawData(const RawDataWeak<T>* rd) {
  std::shared_ptr<RawDataWeak<T>> rd_cloned =
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
      std::make_shared<RawDataWeak<T>>(const_cast<RawDataWeak<T>*>(rd)->getBuffer(), rd->getSize());

  return rd_cloned;
}

template <typename T>
inline std::shared_ptr<RawData> cloneRawData(const RawData* rd) {
  if (const auto* cast_raw_data = dynamic_cast<const RawDataVector<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  }
  if (const auto* cast_raw_data = dynamic_cast<const RawDataNoInit<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  }
  if (const auto* cast_raw_data = dynamic_cast<const RawDataWeak<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  }
  throw std::runtime_error("Can not clone not known RawData derived class");
}

template <>
inline std::shared_ptr<RawData> clone(const RawData& value) {
  std::shared_ptr<RawData> rd_cloned = nullptr;
  const SamplingType current_sampling = value.getSamplingType();
  const DataType current_data = value.getDataType();

  switch (current_sampling) {
    case SamplingType::IQ: {
      switch (current_data) {
        case DataType::DOUBLE: {
          rd_cloned = cloneRawData<std::complex<double>>(&value);
        } break;
        case DataType::FLOAT: {
          rd_cloned = cloneRawData<std::complex<float>>(&value);
        } break;
        case DataType::INT32: {
          rd_cloned = cloneRawData<std::complex<int32_t>>(&value);
        } break;
        case DataType::INT16: {
          rd_cloned = cloneRawData<std::complex<int16_t>>(&value);
        } break;
        default:
          throw std::runtime_error("Can not clone RawData derived class without knowing DataType");
          break;
      }
    } break;

    case SamplingType::RF: {
      switch (current_data) {
        case DataType::DOUBLE: {
          rd_cloned = cloneRawData<double>(&value);
        } break;
        case DataType::FLOAT: {
          rd_cloned = cloneRawData<float>(&value);
        } break;
        case DataType::INT32: {
          rd_cloned = cloneRawData<int32_t>(&value);
        } break;
        case DataType::INT16: {
          rd_cloned = cloneRawData<int16_t>(&value);
        } break;
        default:
          throw std::runtime_error("Can not clone RawData derived class without knowing DataType");
          break;
      }
    } break;
    default:
      throw std::runtime_error("Can not clone RawData derived class without knowing SamplingType");
      break;
  }

  return rd_cloned;
}

template <>
inline GroupData clone(const GroupData& value) {
  GroupData gd_cloned(value);

  if (gd_cloned.raw_data) {
    gd_cloned.raw_data = clone<RawData, std::shared_ptr<RawData>>(*value.raw_data);
  }
  return gd_cloned;
}

template <>
inline Acquisition clone(const Acquisition& value) {
  Acquisition acq_cloned(value);

  for (size_t p_id = 0; p_id < value.probes.size(); ++p_id) {
    acq_cloned.probes[p_id] = clone(value.probes.at(p_id));
  }
  for (size_t ex_id = 0; ex_id < value.excitations.size(); ++ex_id) {
    acq_cloned.excitations[ex_id] = clone(value.excitations.at(ex_id));
  }
  for (size_t g_id = 0; g_id < value.groups.size(); ++g_id) {
    acq_cloned.groups[g_id] = clone(value.groups.at(g_id));
  }
  for (size_t gd_id = 0; gd_id < value.groups_data.size(); ++gd_id) {
    acq_cloned.groups_data[gd_id] = clone(acq_cloned.groups_data.at(gd_id));
    const int32_t g_id = getEltId(value.groups, value.groups_data.at(gd_id).group);
    if (g_id < 0) {
      acq_cloned.groups_data.at(gd_id).group = std::weak_ptr<Group>();
    } else {
      acq_cloned.groups_data.at(gd_id).group = acq_cloned.groups.at(g_id);
    }
  }

  for (size_t g_id = 0; g_id < value.groups.size(); ++g_id) {
    auto& group = acq_cloned.groups.at(g_id);
    if (group) {
      for (auto& event : group->sequence) {
        auto& receive_setup = event.receive_setup;
        auto& transmit_setup = event.transmit_setup;

        int32_t p_id = getEltId(value.probes, receive_setup.probe);
        if (p_id < 0) {
          receive_setup.probe = std::weak_ptr<Probe>();
        } else {
          receive_setup.probe = acq_cloned.probes.at(p_id);
        }

        p_id = getEltId(value.probes, transmit_setup.probe);
        if (p_id < 0) {
          transmit_setup.probe = std::weak_ptr<Probe>();
        } else {
          transmit_setup.probe = acq_cloned.probes.at(p_id);
        }
        for (auto& excitation : transmit_setup.excitations) {
          int32_t const acq_ex_id = getEltId(value.excitations, excitation);
          if (acq_ex_id < 0) {
            excitation = std::weak_ptr<Excitation>();
          } else {
            excitation = acq_cloned.excitations.at(acq_ex_id);
          }
        }
      }
    }
  }

  return acq_cloned;
}

}  // namespace urx::utils
