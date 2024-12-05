#pragma once

#include <memory>
#include <type_traits>

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

}  // namespace urx::utils
