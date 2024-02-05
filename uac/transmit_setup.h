#pragma once

#include <uac/excitation.h>
#include <uac/hw_config.h>

#include <urx/transmit_setup.h>

namespace uac {

struct TransmitSetup : urx::detail::TransmitSetup<Excitation> {
  bool operator==(const TransmitSetup& other) const {
    return urx::detail::TransmitSetup<Excitation>::operator==(other) &&
           hw_config == other.hw_config;
  }

  bool operator!=(const TransmitSetup& other) const { return !operator==(other); }

  HwConfig hw_config;
};

}  // namespace uac
