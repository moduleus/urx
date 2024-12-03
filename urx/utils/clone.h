#pragma once

#include <memory>
#include <type_traits>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/utils/cpp.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

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

// template <typename T>
// ::urx::Vector3D<T> clone(const ::urx::Vector3D<T>& v) {
//   return ::urx::Vector3D<T>(v);
// }

// template <typename T>
// ::urx::Vector2D<T> clone(const ::urx::Vector2D<T>& v) {
//   return ::urx::Vector2D<T>(v);
// }

// ::urx::Version clone(const ::urx::Version& v) { return ::urx::Version(v); }

// ::urx::Element clone(const ::urx::Element& e) { return ::urx::Element(e); }

// ::urx::ElementGeometry clone(const ::urx::ElementGeometry& eg) {
//   return ::urx::ElementGeometry(eg);
// }

// ::urx::DoubleNan clone(const ::urx::DoubleNan& d) { return ::urx::DoubleNan(d); }

// ::urx::ImpulseResponse clone(const ::urx::ImpulseResponse& ir) {
//   return ::urx::ImpulseResponse(ir);
// }

// ::urx::ReceiveSetup clone(const ::urx::ReceiveSetup& rs) { return ::urx::ReceiveSetup(rs); }

// ::urx::TransmitSetup clone(const ::urx::TransmitSetup& ts) { return ::urx::TransmitSetup(ts); }

// ::urx::Wave clone(const ::urx::Wave& w) { return ::urx::Wave(w); }

}  // namespace urx::utils
