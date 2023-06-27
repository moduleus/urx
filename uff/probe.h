#pragma once

#include <uff/element_geometry.h>
#include <uff/impulse_response.h>
#include <uff/transform.h>
#include <uff/uff.h>
#include <cassert>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace uff {

  enum class ProbeType {
    LinearArray = 0,
    RcaArray = 1,
    MatrixArray = 2
  };

/**
 * @brief The UFF Probe class describes a generic ultrsound probe formed by a collection of elements
 */
class Probe {
  // CTOR & DTOR
 public:
  Probe() = default;
  Probe(const Probe&) = default;
  Probe(Probe&&) = default;
  virtual ~Probe() = default;

  // Operators
 public:
  Probe& operator=(const Probe& other) noexcept = default;
  Probe& operator=(Probe&& other) noexcept = default;
  bool operator==(const Probe& other) const {
    return ((_transform == other._transform) && (_focal_length == other._focal_length) &&
            (_elements == other._elements) && (_element_geometries == other._element_geometries) &&
            (_impulse_responses == other._impulse_responses) &&
            (_channel_geometry_valid == other._channel_geometry_valid) &&
            (_channel_geometry == other._channel_geometry));
  }
  inline bool operator!=(const Probe& other) const { return !(*this == other); }

  // Members
 public:
  /* Attitude of the probe in 3D */
  inline const uff::Transform& transform() const { return _transform; }
  inline void setTransform(const uff::Transform& transform) { _transform = transform; }

  /* [optional] For probes with a focal lens, it describes the focal length in m [Az, Ele] */
  inline std::optional<MetadataType> focalLength() const { return _focal_length; }
  inline void setFocalLength(std::optional<MetadataType> focalLength) {
    _focal_length = focalLength;
  }

  /* List elements in the probe */
  inline std::vector<std::optional<uff::Point3D<MetadataType>>>& elements() { return _elements; }
  inline void setElements(const std::vector<std::optional<uff::Point3D<MetadataType>>>& elements) {
    _elements = elements;
    _channel_geometry_valid = false;
  }

  /* [optional] List of unique element geometries in the probe */
  inline std::vector<uff::ElementGeometry>& elementGeometries() { return _element_geometries; }
  inline void setElementGeometries(const std::vector<uff::ElementGeometry>& elementGeometries) {
    _element_geometries = elementGeometries;
  }

  /* [optional] List of unique electromechanical impulse responses of the elements in the probe */
  inline std::vector<uff::ImpulseResponse>& impulseResponses() { return _impulse_responses; }
  inline void setImpulseResponses(const std::vector<uff::ImpulseResponse>& impulseResponses) {
    _impulse_responses = impulseResponses;
  }

  /* Convenience method */
  const std::vector<MetadataType>& channelGeometry() {
    if (!_channel_geometry_valid) {
      computeChannelGeometry();
      _channel_geometry_valid = true;
    }
    return _channel_geometry;
  }

 private:
  void computeChannelGeometry() {
    _channel_geometry.resize(4 * _elements.size());
    auto dst = _channel_geometry.begin();
    for (auto& el : _elements) {
      *dst++ = el.has_value() ? el.value().x() : static_cast<MetadataType>(UFF_NAN);
      *dst++ = el.has_value() ? el.value().y() : static_cast<MetadataType>(UFF_NAN);
      *dst++ = el.has_value() ? el.value().z() : static_cast<MetadataType>(UFF_NAN);
      *dst++ = 0;
    }
  }

 protected:
  virtual void updateElements() = 0;

  // Members
 protected:
  uff::Transform _transform;
  std::optional<MetadataType> _focal_length = std::nullopt;
  std::vector<std::optional<uff::Point3D<MetadataType>>> _elements;
  std::vector<uff::ElementGeometry> _element_geometries;
  std::vector<uff::ImpulseResponse> _impulse_responses;

  bool _channel_geometry_valid = false;
  std::vector<MetadataType> _channel_geometry;
};

}  // namespace uff
