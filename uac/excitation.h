#pragma once

#include <uac/hw_config.h>

#include <urx/detail/double_nan.h>
#include <urx/excitation.h>

namespace uac {

struct Excitation : urx::Excitation {
  bool operator==(const Excitation& other) const {
    return urx::Excitation::operator==(other) && hw_config == other.hw_config;
  }

  bool operator!=(const Excitation& other) const { return !operator==(other); }

  HwConfig hw_config;
};

}  // namespace uac
