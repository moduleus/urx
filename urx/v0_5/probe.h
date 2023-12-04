/*!
 * Copyright Moduleus
 * \file urx/probe.h
 * \brief
 */

#ifndef URX_PROBE_H
#define URX_PROBE_H

#include <cassert>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <urx/v0_5/element.h>
#include <urx/v0_5/element_geometry.h>
#include <urx/v0_5/impulse_response.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/transform.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX Probe class describes a generic ultrsound probe formed by a collection of elements
 */
class Probe : public Object {
  URX_TYPE_MACRO(Probe, Object);

 public:
  Probe() = default;

  Probe(Probe const& other) = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  /* Attitude of the probe in 3D */
  const Transform& transform() const { return m_transform; }
  void setTransform(const Transform& transform) { m_transform = transform; }

  /* [optional] For probes with a focal lens, it describes the focal length in m [Az, Ele] */
  std::optional<MetadataType> focalLength() const { return m_focalLength; }
  void setFocalLength(std::optional<MetadataType> focalLength) { m_focalLength = focalLength; }

  /* List elements in the probe */
  std::vector<Element>& elements() { return m_elements; }
  void setElements(const std::vector<Element>& elements) {
    m_elements = elements;
    _channelGeometryValid = false;
  }

  /* [optional] List of unique element geometries in the probe */
  std::vector<ElementGeometry>& elementGeometries() { return m_elementGeometries; }
  void setElementGeometries(const std::vector<ElementGeometry>& elementGeometries) {
    m_elementGeometries = elementGeometries;
  }

  /* [optional] List of unique electromechanical impulse responses of the elements in the probe */
  std::vector<ImpulseResponse>& impulseResponses() { return m_impulseResponses; }
  void setImpulseResponses(const std::vector<ImpulseResponse>& impulseResponses) {
    m_impulseResponses = impulseResponses;
  }

  /* Convenience method */
  const std::vector<MetadataType>& getChannelGeometry() {
    if (!_channelGeometryValid) {
      // build channel geometry
      _channelGeometry.resize(4 * m_elements.size());
      auto dst = _channelGeometry.begin();
      for (auto& el : m_elements) {
        std::optional<MetadataType> x = el.x();
        std::optional<MetadataType> y = el.y();
        std::optional<MetadataType> z = el.z();
        *dst++ = x.has_value() ? x.value() : static_cast<MetadataType>(URX_NAN);
        *dst++ = y.has_value() ? y.value() : static_cast<MetadataType>(URX_NAN);
        *dst++ = z.has_value() ? z.value() : static_cast<MetadataType>(URX_NAN);
        *dst++ = 0;
      }
      _channelGeometryValid = true;
    }

    return _channelGeometry;
  }

  bool operator==(const Probe& other) const {
    return ((m_transform == other.m_transform) && (m_focalLength == other.m_focalLength) &&
            (m_elements == other.m_elements) &&
            (m_elementGeometries == other.m_elementGeometries) &&
            (m_impulseResponses == other.m_impulseResponses) &&
            (_channelGeometryValid == other._channelGeometryValid) &&
            (_channelGeometry == other._channelGeometry));
  }

  inline bool operator!=(const Probe& other) const { return !(*this == other); }

  Probe& operator=(const Probe& other) = default;

  virtual std::shared_ptr<Probe> clone() { return std::make_shared<Probe>(*this); }

 protected:
  Transform m_transform;
  std::optional<MetadataType> m_focalLength = std::nullopt;
  std::vector<Element> m_elements;
  std::vector<ElementGeometry> m_elementGeometries;
  std::vector<ImpulseResponse> m_impulseResponses;

  bool _channelGeometryValid = false;
  std::vector<MetadataType> _channelGeometry;
};

}  // namespace urx::v0_5

#endif  // URX_PROBE_H
