/*!
 * Copyright Moduleus
 * \file uff_linear_array.h
 * \brief
 */

#ifndef UFF_LINEAR_ARRAY_H
#define UFF_LINEAR_ARRAY_H

// UFF
#include "uff_probe.h"

// System
#include <cstdint>
#include <vector>

namespace uff {

/**
 * @brief The UFF LinearArray class describes a linear array (1D)
 */
class LinearArray : public uff::Probe {
  UFF_TYPE_MACRO(LinearArray, uff::Object);

 public:
  explicit LinearArray(uint32_t numberElements)
      : m_numberElements(numberElements) {
    updateElements();
  }

  void printSelf(std::ostream& os, std::string indent) const override;

  uint32_t numberElements() const { return m_numberElements; }
  void setNumberElements(uint32_t numberElements) {
    m_numberElements = numberElements;
    updateElements();
  }

  std::optional<FloatingType> pitch() { return m_pitch; }
  void setPitch(std::optional<FloatingType> pitch) {
    m_pitch = pitch;
    updateElements();
  }

  std::optional<FloatingType> elementWidth() { return m_elementWidth; }
  void setElementWidth(std::optional<FloatingType> elementWidth) {
    m_elementWidth = elementWidth;
  }

  std::optional<FloatingType> elementHeight() { return m_elementHeight; }
  void setElementHeight(std::optional<FloatingType> elementHeight) {
    m_elementHeight = elementHeight;
  }

  virtual std::shared_ptr<uff::Probe> clone() override {
    return std::make_shared<uff::LinearArray>(*this);
  }

 private:
  // Update elements position
  void updateElements() {
    m_elements.resize(m_numberElements);
    for (uint32_t i = 0; i < m_numberElements; ++i) {
      // element position
      uff::Element element;
      if (m_pitch.has_value()) {
        FloatingType xmin = -m_pitch.value() * (m_numberElements - 1.f) / 2.f;
        element.setX(xmin + i * m_pitch.value());
      } else
        element.setX(0.f);
      element.setY(0.f);
      element.setZ(0.f);
      m_elements[i] = element;
    }
  }

 protected:
  // Number of elements in the array
  uint32_t m_numberElements = 0;

  // Distance between the acoustic ceneter of adyacent elements [m]
  std::optional<FloatingType> m_pitch = std::nullopt;

  // (Optional) Element size in the x-axis [m]
  std::optional<FloatingType> m_elementWidth = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<FloatingType> m_elementHeight = std::nullopt;
};

}  // namespace uff

#endif  // UFF_LINEAR_ARRAY_H