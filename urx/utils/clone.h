#pragma once

#include <memory>
#include <type_traits>

#include <urx/detail/raw_data.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>

template <typename T>
using raw_type = T;

namespace urx::utils {

// Get type associated to SamplingType
template <SamplingType enum_value, typename T>
struct SamplingTypeSelector;

template <typename T>
struct SamplingTypeSelector<SamplingType::IQ, T> {
  using type = std::complex<T>;
};

template <typename T>
struct SamplingTypeSelector<SamplingType::RF, T> {
  using type = T;
};

template <SamplingType enum_value, typename T>
using sampling_type = typename SamplingTypeSelector<enum_value, T>::type;

// Get type associated to DataType
template <DataType enum_value>
struct DataTypeSelector;

template <>
struct DataTypeSelector<DataType::DOUBLE> {
  using type = double;
};

template <>
struct DataTypeSelector<DataType::FLOAT> {
  using type = float;
};

template <>
struct DataTypeSelector<DataType::INT32> {
  using type = int32_t;
};

template <>
struct DataTypeSelector<DataType::INT16> {
  using type = int16_t;
};

template <DataType enum_value>
using data_type = typename DataTypeSelector<enum_value>::type;

// Get type associated to SamplingType and DataType
// Waiting C++20 to be used in clone(GroupData)
template <SamplingType sampling_enum_value, DataType data_enum_value>
using sampling_data_type = sampling_type<sampling_enum_value, data_type<data_enum_value>>;

template <typename T, typename U = T>
U clone(const T& value) {
  if constexpr (std::is_pointer_v<T>) {
    return value ? new std::remove_pointer_t<T>(*value) : nullptr;
  } else if constexpr (utils::is_shared_ptr<T>::value) {
    return value ? std::make_shared<typename T::element_type>(*value) : nullptr;
  } else {
    return value;
  }
}

template <>
Probe clone(const Probe& p) {
  Probe probe_cloned(p);

  for (size_t eg_id = 0; eg_id < p.element_geometries.size(); ++eg_id) {
    probe_cloned.element_geometries[eg_id] = clone(p.element_geometries.at(eg_id));
  }
  for (size_t ir_id = 0; ir_id < p.impulse_responses.size(); ++ir_id) {
    probe_cloned.impulse_responses[ir_id] = clone(p.impulse_responses.at(ir_id));
  }

  for (size_t e_id = 0; e_id < p.elements.size(); ++e_id) {
    int32_t eg_id = getEltId(p.element_geometries, p.elements.at(e_id).element_geometry);
    int32_t ir_id = getEltId(p.impulse_responses, p.elements.at(e_id).impulse_response);
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
std::shared_ptr<Probe> clone(const std::shared_ptr<Probe>& p) {
  return (p == nullptr) ? nullptr : std::make_shared<Probe>(clone(*p));
}

template <typename T>
RawDataVector<T>* cloneRawData(const RawDataVector<T>* rd) {
  std::vector<T> data(static_cast<const T*>(rd->getBuffer()),
                      static_cast<const T*>(rd->getBuffer()) + rd->getSize());

  RawDataVector<T>* rd_cloned = new RawDataVector<T>(data);

  return rd_cloned;
}

template <typename T>
RawDataNoInit<T>* cloneRawData(const RawDataNoInit<T>* rd) {
  RawDataNoInit<T>* rd_cloned = new RawDataNoInit<T>(rd->getSize());
  std::memcpy(rd_cloned->getBuffer(), rd->getBuffer(), rd->getSize() * sizeof(T));

  return rd_cloned;
}

template <typename T>
RawDataWeak<T>* cloneRawData(const RawDataWeak<T>* rd) {
  RawDataWeak<T>* rd_cloned =
      new RawDataWeak<T>(const_cast<RawDataWeak<T>*>(rd)->getBuffer(), rd->getSize());

  return rd_cloned;
}

template <typename T>
RawData* cloneRawData(const RawData* rd) {
  if (const auto* cast_raw_data = dynamic_cast<const RawDataVector<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  } else if (const auto* cast_raw_data = dynamic_cast<const RawDataNoInit<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  } else if (const auto* cast_raw_data = dynamic_cast<const RawDataWeak<T>*>(rd)) {
    return cloneRawData(cast_raw_data);
  } else {
    throw std::runtime_error("Can not clone not known RawData derived class");
  }
}

template <>
RawData* clone(const RawData& rd) {
  RawData* rd_cloned = nullptr;
  SamplingType current_sampling = rd.getSamplingType();
  DataType current_data = rd.getDataType();

  switch (current_sampling) {
    case SamplingType::IQ: {
      switch (current_data) {
        case DataType::DOUBLE: {
          rd_cloned = cloneRawData<std::complex<double>>(&rd);
        } break;
        case DataType::FLOAT: {
          rd_cloned = cloneRawData<std::complex<float>>(&rd);
        } break;
        case DataType::INT32: {
          rd_cloned = cloneRawData<std::complex<int32_t>>(&rd);
        } break;
        case DataType::INT16: {
          rd_cloned = cloneRawData<std::complex<int16_t>>(&rd);
        } break;
        default:
          throw std::runtime_error("Can not clone RawData derived class without knowing DataType");
          break;
      }
    } break;

    case SamplingType::RF: {
      switch (current_data) {
        case DataType::DOUBLE: {
          rd_cloned = cloneRawData<double>(&rd);
        } break;
        case DataType::FLOAT: {
          rd_cloned = cloneRawData<float>(&rd);
        } break;
        case DataType::INT32: {
          rd_cloned = cloneRawData<int32_t>(&rd);
        } break;
        case DataType::INT16: {
          rd_cloned = cloneRawData<int16_t>(&rd);
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
GroupData clone(const GroupData& gd) {
  GroupData gd_cloned(gd);

  if (gd_cloned.raw_data) {
    gd_cloned.raw_data = std::shared_ptr<RawData>(clone<RawData, RawData*>(*gd.raw_data));
  }
  return gd_cloned;
}

// template <>
// Acquisition clone(const Acquisition& acq) {
//   Acquisition acq_cloned(acq);

//   for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
//     acq_cloned.probes[p_id] = clone(acq_cloned.probes.at(p_id));
//   }
//   for (size_t ex_id = 0; ex_id < acq.excitations.size(); ++ex_id) {
//     acq_cloned.excitations[ex_id] = clone(acq_cloned.excitations.at(ex_id));
//   }
//   for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
//     acq_cloned.groups[g_id] = clone(acq_cloned.groups.at(g_id));
//   }

//   for (size_t gd_id = 0; gd_id < acq.groups_data.size(); ++gd_id) {
//     int32_t g_id = getEltId(acq.groups, acq.groups_data.at(gd_id).group);
//     if (g_id < 0) {
//       acq_cloned.groups_data.at(gd_id).group = std::weak_ptr<Group>();
//     } else {
//       acq_cloned.groups_data.at(gd_id).group = acq_cloned.groups.at(g_id);
//     }
//   }
//   return acq_cloned;
// }

}  // namespace urx::utils
