#pragma once

#include <iosfwd>
#include <memory>
#include <string>

#include <urx/urx.h>
#include <urx/vector.h>

namespace urx {

struct Transform {
  bool operator==(const Transform& other) const = default;

  Vector3D<double> rotation;
  Vector3D<double> translation;
};

}  // namespace urx
