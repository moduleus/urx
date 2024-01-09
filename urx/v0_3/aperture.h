#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/excitation.h>
#include <urx/v0_3/position.h>
#include <urx/v0_3/transform.h>
#include <urx/v0_3/wave_type.h>

namespace urx::v0_3 {

struct Aperture {
  Transform origin;
  std::string window;
  double f_number;
  double fixed_size;
};

}  // namespace urx::v0_3
