#pragma once

#include <uff/transform.h>
#include <uff/uff.h>
#include <iosfwd>
#include <optional>
#include <string>
#include <utility>
namespace uff {

/**
 * @brief UFF class to define analytically the aperture use in an ultrasound wave
 */
class Aperture {
  // CTOR & DTOR
 public:
  Aperture() = default;
  Aperture(const Aperture&) = default;
  Aperture(Aperture&&) = default;
  ~Aperture() = default;

  // Operators
 public:
  Aperture& operator=(const Aperture& other) noexcept = default;
  Aperture& operator=(Aperture&& other) noexcept = default;
  bool operator==(const Aperture& other) const {
    return ((_origin == other._origin) && (_window == other._window) &&
            (_f_number == other._f_number) && (_fixed_size == other._fixed_size));
  }
  inline bool operator!=(const Aperture& other) const { return !(*this == other); }

  // Accessors
 public:
  inline const uff::Transform& origin() const { return _origin; }
  inline void setOrigin(const uff::Transform& origin) { _origin = origin; }

  inline const std::optional<std::string>& window() const { return _window; }
  inline void setWindow(const std::optional<std::string>& window) { _window = window; }

  inline std::optional<MetadataType> fNumber() const { return _f_number; }
  inline void setFNumber(std::optional<MetadataType> f_number) { _f_number = f_number; }

  inline std::optional<MetadataType> fixedSize() const { return _fixed_size; }
  inline void setFixedSize(std::optional<MetadataType> fixedSize) { _fixed_size = fixedSize; }

  // Members
 private:
  // Location of the aperture center in space
  uff::Transform _origin;

  // String defining the apodization window type and parameter (e.g., 'Rectangular', 'Hamming', 'Gauss(8)', 'Tukey(0.5)')
  std::optional<std::string> _window = std::nullopt;

  // Desired F-number of the aperture [Az, El]
  std::optional<MetadataType> _f_number = std::nullopt;

  // If non-zero, this overwrites the size of the aperture in m [Az, El]
  std::optional<MetadataType> _fixed_size = std::nullopt;
};

}  // namespace uff
