/*!
 * Copyright Moduleus
 * \file uff/probe.h
 * \brief
 */

#ifndef UFF_PROBE_H
#define UFF_PROBE_H

// UFF
#include "uff/element.h"
#include "uff/element_geometry.h"
#include "uff/impulse_response.h"
#include "uff/object.h"
#include "uff/transform.h"

// System
#include <memory>
#include <optional>
#include <vector>

namespace uff {

/**
 * @brief The UFF Probe class describes a generic ultrsound probe formed by a collection of elements
 */
class Probe : public uff::Object {
  UFF_TYPE_MACRO(Probe, uff::Object);

 public:
  Probe() = default;

  void printSelf(std::ostream& os, std::string indent) const override;

  /* Attitude of the probe in 3D */
  const uff::Transform& transform() const { return m_transform; }
  void setTransform(const uff::Transform& transform) { m_transform = transform; }

  /* [optional] For probes with a focal lens, it describes the focal length in m [Az, Ele] */
  std::optional<MetadataType> focalLength() const { return m_focalLength; }
  void setFocalLength(std::optional<MetadataType> focalLength) { m_focalLength = focalLength; }

  /* List elements in the probe */
  std::vector<uff::Element>& elements() { return m_elements; }
  void setElements(const std::vector<uff::Element>& elements) {
    m_elements = elements;
    _channelGeometryValid = false;
  }

  /* [optional] List of unique element geometries in the probe */
  std::vector<uff::ElementGeometry>& elementGeometries() { return m_elementGeometries; }
  void setElementGeometries(const std::vector<uff::ElementGeometry>& elementGeometries) {
    m_elementGeometries = elementGeometries;
  }

  /* [optional] List of unique electromechanical impulse responses of the elements in the probe */
  std::vector<uff::ImpulseResponse>& impulseResponses() { return m_impulseResponses; }
  void setImpulseResponses(const std::vector<uff::ImpulseResponse>& impulseResponses) {
    m_impulseResponses = impulseResponses;
  }

  /* Convenience method */
  const std::vector<MetadataType>& getChannelGeometry() {
    if (!_channelGeometryValid) {
      // build channel geometry
      _channelGeometry.resize(4 * m_elements.size());
      auto dst = _channelGeometry.begin();
      for (auto& el : m_elements) {
        *dst++ = el.x().has_value() ? el.x().value() : static_cast<MetadataType>(UFF_NAN);
        *dst++ = el.y().has_value() ? el.y().value() : static_cast<MetadataType>(UFF_NAN);
        *dst++ = el.z().has_value() ? el.z().value() : static_cast<MetadataType>(UFF_NAN);
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

  Probe& operator=(const Probe& other) {
    m_transform = other.m_transform;
    m_focalLength = other.m_focalLength;
    m_elements = other.m_elements;
    m_elementGeometries = other.m_elementGeometries;
    m_impulseResponses = other.m_impulseResponses;
    _channelGeometryValid = other._channelGeometryValid;
    _channelGeometry = other._channelGeometry;
    assert(*this == other);
    return *this;
  }

  virtual std::shared_ptr<uff::Probe> clone() { return std::make_shared<uff::Probe>(*this); }

 protected:
  uff::Transform m_transform;
  std::optional<MetadataType> m_focalLength = std::nullopt;
  std::vector<uff::Element> m_elements;
  std::vector<uff::ElementGeometry> m_elementGeometries;
  std::vector<uff::ImpulseResponse> m_impulseResponses;

  bool _channelGeometryValid = false;
  std::vector<MetadataType> _channelGeometry;
};

}  // namespace uff

#include "uff/linear_array.h"
#include "uff/matrix_array.h"
#include "uff/rca_array.h"

#endif  // UFF_PROBE_H
