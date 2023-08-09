#pragma once

// UFF
#include <uff/igroup.h>

// System
#include <string>

namespace uff {

class GroupLink {
 public:
  // CTOR & DTOR
  GroupLink() = default;
  GroupLink(std::weak_ptr<IGroup> source, std::weak_ptr<IGroup> destination)
      : _source(std::move(source)), _destination(std::move(destination)) {}
  GroupLink(const GroupLink&) = default;
  GroupLink(GroupLink&&) noexcept = default;
  ~GroupLink() = default;

  // Operators
  GroupLink& operator=(const GroupLink& other) noexcept = default;
  GroupLink& operator=(GroupLink&& other) noexcept = default;
  bool operator==(const GroupLink& other) const {
    return (_source.expired() == other._source.expired()) &&
           (_source.expired() || *(_source.lock()) == *(other._source.lock())) &&
           (_destination.expired() == other._destination.expired()) &&
           (_destination.expired() || *(_destination.lock()) == *(other._destination.lock()));
  }
  inline bool operator!=(const GroupLink& other) const { return !(*this == other); }

  // Accessors
  inline const std::weak_ptr<IGroup>& source() const { return _source; }
  inline void setSource(const std::weak_ptr<IGroup>& source) { _source = source; }

  inline std::weak_ptr<IGroup> destination() const { return _destination; }
  inline void setDestination(const std::weak_ptr<IGroup>& destination) {
    _destination = destination;
  }

  // Members
 private:
  std::weak_ptr<IGroup> _source;
  std::weak_ptr<IGroup> _destination;
};

}  // namespace uff
