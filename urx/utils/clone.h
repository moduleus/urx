#pragma once

#include <memory>
#include <type_traits>

#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/cpp.h>

namespace urx::utils {

template <typename T>
T clone(const T& value) {
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

}  // namespace urx::utils
