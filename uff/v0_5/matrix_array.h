/*!
 * Copyright Moduleus
 * \file uff/matrix_array.h
 * \brief
 */

#ifndef UFF_MATRIX_ARRAY_H
#define UFF_MATRIX_ARRAY_H

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "uff/v0_5/element.h"
#include "uff/v0_5/object.h"
#include "uff/v0_5/probe.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief The UFF MatrixArray class describes a 2D matrix array of elements
 * Element[0] has the minimum x/y coordinates. Element[1] has the same y-coordinates as
 * element[0] but a higher x-coordinates.
 */
class MatrixArray : public uff::Probe {
  UFF_TYPE_MACRO(MatrixArray, uff::Object);

 public:
  MatrixArray() = default;
  MatrixArray(const uint32_t& nbElementsX, const uint32_t& nbElementsY, const MetadataType& pitchX,
              const MetadataType& picthY) {
    m_numberElementsX = nbElementsX;
    m_numberElementsY = nbElementsY;
    m_pitchX = pitchX;
    m_pitchY = picthY;
    updateElements();
  }

  void printSelf(std::ostream& os, const std::string& indent) const override;

  uint32_t numberElementsX() const { return m_numberElementsX; }
  void setNumberElementsX(uint32_t numberElementsX) {
    m_numberElementsX = numberElementsX;
    updateElements();
  }

  uint32_t numberElementsY() const { return m_numberElementsY; }
  void setNumberElementsY(uint32_t numberElementsY) {
    m_numberElementsY = numberElementsY;
    updateElements();
  }

  MetadataType pitchX() const { return m_pitchX; }
  void setPitchX(const MetadataType& pitchX) {
    m_pitchX = pitchX;
    updateElements();
  }

  MetadataType pitchY() const { return m_pitchY; }
  void setPitchY(const MetadataType& pitchY) {
    m_pitchY = pitchY;
    updateElements();
  }

  std::optional<MetadataType> elementWidth() { return m_elementWidth; }
  void setElementWidth(std::optional<MetadataType> elementWidth) { m_elementWidth = elementWidth; }

  std::optional<MetadataType> elementHeight() { return m_elementHeight; }
  void setElementHeight(std::optional<MetadataType> elementHeight) {
    m_elementHeight = elementHeight;
  }

  std::shared_ptr<uff::Probe> clone() override { return std::make_shared<uff::MatrixArray>(*this); }

 private:
  // Update elements position
  void updateElements() {
    m_elements.resize(static_cast<size_t>(m_numberElementsX) * m_numberElementsY);

    MetadataType xmin = -m_pitchX * (m_numberElementsX - 1.f) / 2.f;
    MetadataType ymin = -m_pitchY * (m_numberElementsY - 1.f) / 2.f;
    for (uint32_t i = 0; i < m_numberElementsY; i++) {
      for (uint32_t j = 0; j < m_numberElementsX; j++) {
        uff::Element element;
        element.setX(xmin + j * m_pitchX);
        element.setY(ymin + i * m_pitchY);
        element.setZ(0.f);
        m_elements[static_cast<size_t>(j) + static_cast<size_t>(i) * m_numberElementsY] = element;
      }
    }
  }

 protected:
  // Number of elements in the x-axis
  uint32_t m_numberElementsX = 0;

  // Number of elements in the y-axis
  uint32_t m_numberElementsY = 0;

  // Distance between the acoustic center of adyacent elements along the x-axis [m]
  MetadataType m_pitchX = 0;

  // Distance between the acoustic center of adyacent elements along the y-axis [m]
  MetadataType m_pitchY = 0;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> m_elementWidth = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> m_elementHeight = std::nullopt;
};

}  // namespace uff::v0_5

#endif  // UFF_MATRIX_ARRAY_H