#pragma once

#include <uff/element.h>
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

enum class ProbeType { LinearArray = 0, RcaArray = 1, MatrixArray = 2 };

/**
 * @brief The UFF Probe class describes a generic ultrsound probe formed by a collection of elements
 */
class Probe {
 public:
  // CTOR & DTOR
  Probe() = default;
  Probe(const Transform& transform, const std::vector<Element>& elements,
        const std::vector<std::shared_ptr<ElementGeometry>>& element_geometries = {},
        const std::vector<std::shared_ptr<ImpulseResponse>>& impulse_responses = {})
      : _transform(transform),
        _elements(elements),
        _element_geometries(element_geometries),
        _impulse_responses(impulse_responses) {}
  Probe(const Probe&) = default;
  Probe(Probe&&) = default;
  virtual ~Probe() = default;

  // Operators
  Probe& operator=(const Probe& other) noexcept = default;
  Probe& operator=(Probe&& other) noexcept = default;
  bool operator==(const Probe& other) const {
    return ((_transform == other._transform) && (_focal_length == other._focal_length) &&
            (_elements == other._elements) && (_element_geometries == other._element_geometries) &&
            (_impulse_responses == other._impulse_responses));
  }
  inline bool operator!=(const Probe& other) const { return !(*this == other); }

  // Members
 public:
  inline const Transform& transform() const { return _transform; }
  inline void setTransform(const Transform& transform) { _transform = transform; }

  inline std::optional<MetadataType> focalLength() const { return _focal_length; }
  inline void setFocalLength(const std::optional<MetadataType>& focal_length) {
    _focal_length = focal_length;
  }

  inline std::vector<Element>& elements() { return _elements; }
  inline void setElements(const std::vector<Element>& elements) { _elements = elements; }

  inline std::vector<std::shared_ptr<ElementGeometry>>& elementGeometries() {
    return _element_geometries;
  }
  inline void setElementGeometries(
      const std::vector<std::shared_ptr<ElementGeometry>>& elementGeometries) {
    _element_geometries = elementGeometries;
  }

  inline std::vector<std::shared_ptr<ImpulseResponse>>& impulseResponses() {
    return _impulse_responses;
  }
  inline void setImpulseResponses(
      const std::vector<std::shared_ptr<ImpulseResponse>>& impulseResponses) {
    _impulse_responses = impulseResponses;
  }

  // Members
 protected:
  // Attitude of the probe in 3D
  Transform _transform;

  // List all the elements in the probe
  std::vector<Element> _elements;

  // [optional] For probes with a focal lens, it describes the focal length in m [Az, Ele]
  std::optional<MetadataType> _focal_length = std::nullopt;

  // [optional] List of all unique element geometries in the probe
  std::vector<std::shared_ptr<ElementGeometry>> _element_geometries;

  // [optional] List of unique electromechanical impulse responses of the elements in the probe
  std::vector<std::shared_ptr<ImpulseResponse>> _impulse_responses;
};

}  // namespace uff
