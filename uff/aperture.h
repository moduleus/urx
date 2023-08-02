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
class Aperture {
 public:
  // CTOR & DTOR
  Aperture() = default;
  Aperture(const Aperture&) = default;
  Aperture(Aperture&&) = default;
  ~Aperture() = default;

  // Operators
  Aperture& operator=(const Aperture& other) noexcept = default;
  Aperture& operator=(Aperture&& other) noexcept = default;
  bool operator==(const Aperture& other) const {
    return ((_origin == other._origin) && (_window == other._window) &&
            (_f_number == other._f_number) && (_fixed_size == other._fixed_size));
  }
  inline bool operator!=(const Aperture& other) const { return !(*this == other); }

  // Accessors
  inline const Transform& origin() const { return _origin; }
  inline void setOrigin(const Transform& origin) { _origin = origin; }

  inline const std::string& window() const { return _window; }
  inline void setWindow(const std::string& window) { _window = window; }

  inline MetadataType fNumber() const { return _f_number; }
  inline void setFNumber(MetadataType f_number) { _f_number = f_number; }

  inline MetadataType fixedSize() const { return _fixed_size; }
  inline void setFixedSize(MetadataType fixedSize) { _fixed_size = fixedSize; }

  // Members
 private:
  // Location of the aperture center in space
  Transform _origin;

  // String defining the apodization window type and parameter (e.g., 'Rectangular', 'Hamming', 'Gauss(8)', 'Tukey(0.5)')
  std::string _window;

  // Desired F-number of the aperture [Az, El]
  MetadataType _f_number;

  // If non-zero, this overwrites the size of the aperture in m [Az, El]
  MetadataType _fixed_size;
};

}  // namespace uff
