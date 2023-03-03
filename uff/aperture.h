/*!
 * Copyright Moduleus
 * \file uff/aperture.h
 * \brief
 */

#ifndef UFF_APERTURE_H
#define UFF_APERTURE_H

#include <uff/object.h>
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
class Aperture : public uff::Object {
  UFF_TYPE_MACRO(Aperture, uff::Object);

 public:
  Aperture() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  // Origin
  const uff::Transform& origin() const { return m_origin; }
  void setOrigin(const uff::Transform& origin) { m_origin = origin; }

  // Window
  const std::optional<std::string>& window() const { return m_window; }
  void setWindow(const std::optional<std::string>& window) { m_window = window; }

  // F number
  std::optional<MetadataType> fNumber() const { return m_fNumber; }
  void setFNumber(std::optional<MetadataType> f_number) { m_fNumber = f_number; }

  // Fixed size
  std::optional<MetadataType> fixedSize() const { return m_fixedSize; }
  void setFixedSize(std::optional<MetadataType> fixedSize) { m_fixedSize = fixedSize; }

  bool operator==(const Aperture& other) const {
    return ((m_origin == other.m_origin) && (m_window == other.m_window) &&
            (m_fNumber == other.m_fNumber) && (m_fixedSize == other.m_fixedSize));
  }

  inline bool operator!=(const Aperture& other) const { return !(*this == other); }

 private:
  // Location of the aperture center in space
  uff::Transform m_origin;

  // String defining the apodization window type and parameter (e.g., 'Rectangular', 'Hamming', 'Gauss(8)', 'Tukey(0.5)')
  std::optional<std::string> m_window = std::nullopt;

  // Desired F-number of the aperture [Az, El]
  std::optional<MetadataType> m_fNumber = std::nullopt;

  // If non-zero, this overwrites the size of the aperture in m [Az, El]
  std::optional<MetadataType> m_fixedSize = std::nullopt;
};

}  // namespace uff

#endif  // UFF_APERTURE_H
