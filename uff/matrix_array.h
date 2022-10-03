/*!
 * Copyright Moduleus
 * \file uff/matrix_array.h
 * \brief
 */

#ifndef UFF_MATRIX_ARRAY_H
#define UFF_MATRIX_ARRAY_H

// UFF
#include "uff/probe.h"

// System
#include <cstdint>
#include <optional>
#include <vector>

namespace uff {

/**
 * @brief The UFF MatrixArray class describes a 2D matrix array of elements
 * Element[0] has the minimum x/y coordinates. Element[1] has the same y-coordinates as
 * element[0] but a higher x-coordinates.
 */
class MatrixArray : public uff::Probe {
  UFF_TYPE_MACRO(MatrixArray, uff::Object);

 public:
  MatrixArray() {}

  void printSelf(std::ostream& os, std::string indent) const override;

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

  std::optional<MetadataType> pitchX() { return m_pitchX; }
  void setPitchX(std::optional<MetadataType> pitchX) {
    m_pitchX = pitchX;
    updateElements();
  }

  std::optional<MetadataType> pitchY() { return m_pitchY; }
  void setPitchY(std::optional<MetadataType> pitchY) {
    m_pitchY = pitchY;
    updateElements();
  }

  std::optional<MetadataType> elementWidth() { return m_elementWidth; }
  void setElementWidth(std::optional<MetadataType> elementWidth) { m_elementWidth = elementWidth; }

  std::optional<MetadataType> elementHeight() { return m_elementHeight; }
  void setElementHeight(std::optional<MetadataType> elementHeight) {
    m_elementHeight = elementHeight;
  }

  virtual std::shared_ptr<uff::Probe> clone() override {
    return std::make_shared<uff::MatrixArray>(*this);
  }

 private:
  // Update elements position
  void updateElements() {
    m_elements.resize((size_t)m_numberElementsX * m_numberElementsY);

    MetadataType pitchX = m_pitchX.has_value() ? m_pitchX.value() : UFF_NAN;
    MetadataType pitchY = m_pitchY.has_value() ? m_pitchY.value() : UFF_NAN;

    MetadataType xmin = -pitchX * (m_numberElementsX - 1.f) / 2.f;
    MetadataType ymin = -pitchY * (m_numberElementsY - 1.f) / 2.f;
    for (uint32_t i = 0; i < m_numberElementsY; i++) {
      for (uint32_t j = 0; j < m_numberElementsX; j++) {
        uff::Element element;
        if (m_pitchX.has_value())
          element.setX(xmin + j * pitchX);
        else
          element.setX(std::nullopt);

        if (m_pitchY.has_value())
          element.setY(ymin + i * pitchY);
        else
          element.setY(std::nullopt);

        element.setZ(0.f);
        m_elements[(size_t)j + (size_t)i * m_numberElementsY] = element;
      }
    }
  }

 protected:
  // Number of elements in the x-axis
  uint32_t m_numberElementsX = 0;

  // Number of elements in the y-axis
  uint32_t m_numberElementsY = 0;

  // Distance between the acoustic center of adyacent elements along the x-axis [m]
  std::optional<MetadataType> m_pitchX = std::nullopt;

  // Distance between the acoustic center of adyacent elements along the y-axis [m]
  std::optional<MetadataType> m_pitchY = std::nullopt;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> m_elementWidth = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> m_elementHeight = std::nullopt;
};

}  // namespace uff

#endif  // UFF_MATRIX_ARRAY_H