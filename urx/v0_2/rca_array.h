// Copyright Moduleus

#ifndef URX_RCA_ARRAY_H
#define URX_RCA_ARRAY_H

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>

#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX RCA Array class describes a RCA (Row Column Address) probe
 */
class RcaArray : public Probe {
  URX_TYPE_MACRO(RcaArray, Object);

 public:
  explicit RcaArray(uint32_t numberElementsX, uint32_t numberElementsy)
      : m_numberElementsX(numberElementsX), m_numberElementsY(numberElementsy) {
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

  MetadataType pitchY() const { return m_pitchX; }
  void setPitchY(const MetadataType& pitchY) {
    m_pitchY = pitchY;
    updateElements();
  }

  std::optional<MetadataType> elementWidthX() const { return m_elementWidthX; }
  void setElementWidthX(std::optional<MetadataType> elementWidthX) {
    m_elementWidthX = elementWidthX;
  }

  std::optional<MetadataType> elementWidthY() const { return m_elementWidthY; }
  void setElementWidthY(std::optional<MetadataType> elementWidthY) {
    m_elementWidthY = elementWidthY;
  }

  std::optional<MetadataType> elementHeightX() const { return m_elementHeightX; }
  void setElementHeightX(std::optional<MetadataType> elementHeightX) {
    m_elementHeightX = elementHeightX;
  }

  std::optional<MetadataType> elementHeightY() const { return m_elementHeightY; }
  void setElementHeightY(std::optional<MetadataType> elementHeightY) {
    m_elementHeightY = elementHeightY;
  }

  std::shared_ptr<Probe> clone() override { return std::make_shared<RcaArray>(*this); }

 private:
  // Update elements position
  void updateElements();

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
  std::optional<MetadataType> m_elementWidthX = std::nullopt;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> m_elementWidthY = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> m_elementHeightX = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> m_elementHeightY = std::nullopt;
};

}  // namespace urx::v0_2

#endif  // URX_RCA_ARRAY_H