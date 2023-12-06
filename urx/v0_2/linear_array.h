/*!
 * Copyright Moduleus
 * \file urx/linear_array.h
 * \brief
 */

#ifndef URX_LINEAR_ARRAY_H
#define URX_LINEAR_ARRAY_H

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <urx/v0_2/element.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX LinearArray class describes a linear array (1D)
 */
class LinearArray : public Probe {
  URX_TYPE_MACRO(LinearArray, Object);

 public:
  explicit LinearArray(uint32_t numberElements) : m_numberElements(numberElements) {
    updateElements();
  }

  void printSelf(std::ostream &os, const std::string &indent) const override;

  uint32_t numberElements() const { return m_numberElements; }
  void setNumberElements(uint32_t numberElements) {
    m_numberElements = numberElements;
    updateElements();
  }

  MetadataType pitch() const { return m_pitch; }
  void setPitch(const MetadataType &pitch) {
    m_pitch = pitch;
    updateElements();
  }

  std::optional<MetadataType> elementWidth() { return m_elementWidth; }
  void setElementWidth(std::optional<MetadataType> elementWidth) { m_elementWidth = elementWidth; }

  std::optional<MetadataType> elementHeight() { return m_elementHeight; }
  void setElementHeight(std::optional<MetadataType> elementHeight) {
    m_elementHeight = elementHeight;
  }

  std::shared_ptr<Probe> clone() override { return std::make_shared<LinearArray>(*this); }

 private:
  // Update elements position
  void updateElements() {
    m_elements.resize(m_numberElements);
    for (uint32_t i = 0; i < m_numberElements; ++i) {
      // element position
      Element element;
      const MetadataType xmin = -m_pitch * static_cast<float>(m_numberElements - 1) / 2.f;
      element.setX(xmin + i * m_pitch);
      element.setY(0.f);
      element.setZ(0.f);
      m_elements[i] = element;
    }
  }

 protected:
  // Number of elements in the array
  uint32_t m_numberElements = 0;

  // Distance between the acoustic ceneter of adyacent elements [m]
  MetadataType m_pitch = 0;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> m_elementWidth = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> m_elementHeight = std::nullopt;
};

}  // namespace urx::v0_2

#endif  // URX_LINEAR_ARRAY_H