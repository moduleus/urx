#pragma once

#include <uac/hw_config.h>

#include <urx/receive_setup.h>
namespace uac {

struct ReceiveSetup : urx::ReceiveSetup {
  bool operator==(const ReceiveSetup& other) const {
    return urx::ReceiveSetup::operator==(other) && hw_config == other.hw_config;
  }

  bool operator!=(const ReceiveSetup& other) const { return !operator==(other); }

  HwConfig hw_config;
};

}  // namespace uac
