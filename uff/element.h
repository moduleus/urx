#pragma once

#include <uff/element_geometry.h>
#include <uff/impulse_response.h>
#include <uff/transform.h>
#include <uff/uff.h>

#include <memory>

namespace uff {

class Element {
 public:
  // CTOR & DTOR
  Element() = default;
  Element(const Transform& transform,
          const std::weak_ptr<ElementGeometry>& element_geometry = std::weak_ptr<ElementGeometry>(),
          const std::weak_ptr<ImpulseResponse>& impulse_response = std::weak_ptr<ImpulseResponse>())
      : _transform(transform),
        _element_geometry(element_geometry),
        _impluse_response(impulse_response) {}
  Element(const Element&) = default;
  Element(Element&&) = default;
  ~Element() = default;

  // Operators
  Element& operator=(const Element& other) noexcept = default;
  Element& operator=(Element&& other) noexcept = default;
  inline bool operator==(const Element& other) const {
    return (_transform == other._transform) &&
           (_element_geometry.expired() == other._element_geometry.expired()) &&
           (_element_geometry.expired() ||
            *(_element_geometry.lock()) == *(other._element_geometry.lock())) &&
           (_impluse_response.expired() == other._impluse_response.expired()) &&
           (_impluse_response.expired() ||
            *(_impluse_response.lock()) == *(other._impluse_response.lock()));
  }
  inline bool operator!=(const Element& other) const { return !(*this == other); }

  // Accessors
  inline Transform transform() const { return _transform; }
  inline void setTransform(const Transform& transform) { _transform = transform; }

  // Members
 private:
  Transform _transform;

  std::weak_ptr<ElementGeometry> _element_geometry;

  std::weak_ptr<ImpulseResponse> _impluse_response;
};

}  // namespace uff