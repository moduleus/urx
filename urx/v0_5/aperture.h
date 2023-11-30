/*!
 * Copyright Moduleus
 * \file urx/aperture.h
 * \brief
 */

#ifndef URX_APERTURE_H
#define URX_APERTURE_H

#include <iosfwd>
#include <optional>
#include <string>
#include <utility>

#include <urx/v0_5/object.h>
#include <urx/v0_5/transform.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief URX class to define analytically the aperture use in an ultrasound wave
 */
class Aperture : public urx::Object {
  URX_TYPE_MACRO(Aperture, urx::Object);

 public:
  Aperture() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  // Origin
  const urx::Transform& origin() const { return m_origin; }
  void setOrigin(const urx::Transform& origin) { m_origin = origin; }

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
  urx::Transform m_origin;

  // String defining the apodization window type and parameter (e.g., 'Rectangular', 'Hamming', 'Gauss(8)', 'Tukey(0.5)')
  std::optional<std::string> m_window = std::nullopt;

  // Desired F-number of the aperture [Az, El]
  std::optional<MetadataType> m_fNumber = std::nullopt;

  // If non-zero, this overwrites the size of the aperture in m [Az, El]
  std::optional<MetadataType> m_fixedSize = std::nullopt;
};

}  // namespace urx::v0_5

#endif  // URX_APERTURE_H
