#pragma once

#include <algorithm>
#include <complex>
#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transmit_setup.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>

namespace urx::utils::detail {

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
    std::optional<size_t> const eg_id = common::getElementIndex(
        value.element_geometries, value.elements.at(e_id).element_geometry, false);
    std::optional<size_t> const ir_id = common::getElementIndex(
        value.impulse_responses, value.elements.at(e_id).impulse_response, false);
    if (!eg_id.has_value()) {
      probe_cloned.elements.at(e_id).element_geometry = std::weak_ptr<ElementGeometry>();
    } else {
      probe_cloned.elements.at(e_id).element_geometry =
          probe_cloned.element_geometries.at(eg_id.value());
    }
    if (!ir_id.has_value()) {
      probe_cloned.elements.at(e_id).impulse_response = std::weak_ptr<ImpulseResponse>();
    } else {
      probe_cloned.elements.at(e_id).impulse_response =
          probe_cloned.impulse_responses.at(ir_id.value());
    }
  }
  return probe_cloned;
}

template <typename T>
inline std::shared_ptr<RawDataVector<T>> cloneRawData(const RawDataVector<T>* rd) {
  std::vector<T> data(static_cast<const T*>(rd->getBuffer()),
                      static_cast<const T*>(rd->getBuffer()) + rd->getSize());

  std::shared_ptr<RawDataVector<T>> rd_cloned = std::make_shared<RawDataVector<T>>(std::move(data));

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
    const std::optional<size_t> g_id =
        common::getElementIndex(value.groups, value.groups_data.at(gd_id).group, true);
    if (!g_id.has_value()) {
      acq_cloned.groups_data.at(gd_id).group = std::weak_ptr<Group>();
    } else {
      acq_cloned.groups_data.at(gd_id).group = acq_cloned.groups.at(*g_id);
    }
  }

  for (size_t g_id = 0; g_id < value.groups.size(); ++g_id) {
    auto& group_cloned = acq_cloned.groups.at(g_id);
    if (group_cloned) {
      for (auto& event_cloned : group_cloned->sequence) {
        auto& receive_setup_cloned = event_cloned.receive_setup;
        auto& transmit_setup_cloned = event_cloned.transmit_setup;

        std::optional<size_t> p_id =
            common::getElementIndex(value.probes, receive_setup_cloned.probe, false);
        if (!p_id.has_value()) {
          receive_setup_cloned.probe = std::weak_ptr<Probe>();
        } else {
          receive_setup_cloned.probe = acq_cloned.probes.at(*p_id);
        }

        p_id = common::getElementIndex(value.probes, transmit_setup_cloned.probe, false);
        if (!p_id.has_value()) {
          transmit_setup_cloned.probe = std::weak_ptr<Probe>();
        } else {
          transmit_setup_cloned.probe = acq_cloned.probes.at(*p_id);
        }
        for (auto& excitation_cloned : transmit_setup_cloned.excitations) {
          std::optional<size_t> const acq_ex_id =
              common::getElementIndex(value.excitations, excitation_cloned, false);
          if (!acq_ex_id.has_value()) {
            excitation_cloned = std::weak_ptr<Excitation>();
          } else {
            excitation_cloned = acq_cloned.excitations.at(*acq_ex_id);
          }
        }
      }
    }
  }

  return acq_cloned;
}

template <>
inline Dataset clone(const Dataset& value) {
  Dataset dataset_cloned(value);
  dataset_cloned.acquisition = clone(value.acquisition);

  return dataset_cloned;
}

}  // namespace urx::utils::detail
