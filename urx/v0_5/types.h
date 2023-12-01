#pragma once

/*!
 * Copyright Moduleus
 * \file urx/types.h
 * \brief
 */

#include <iosfwd>

namespace urx::v0_5 {

enum class WaveType {
  CONVERGING_WAVE = 0,
  DIVERGING_WAVE = 1,
  PLANE_WAVE = 2,
  CYLINDRICAL_WAVE = 3
};

std::ostream& operator<<(std::ostream& out, const WaveType& value);

}  // namespace urx::v0_5