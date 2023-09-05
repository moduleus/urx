#pragma once

#include <uff/transform.h>
#include <uff/uff.h>
#include <iosfwd>
#include <string>
#include <utility>
namespace uff {

/**
 * @brief UFF class to define analytically the aperture use in an ultrasound wave
 */
struct Aperture {
  auto operator<=>(const Aperture& other) const = default;

  // Location of the aperture center in space
  Transform origin;

  // String defining the apodization window type and parameter (e.g., 'Rectangular', 'Hamming', 'Gauss(8)', 'Tukey(0.5)')
  std::string window = "";

  // Desired F-number of the aperture [Az, El]
  double f_number = UFF_NAN;

  // If non-zero, this overwrites the size of the aperture in m [Az, El]
  double fixed_size = UFF_NAN;
};

}  // namespace uff
