#pragma once

#include <uff/group.h>
#include <uff/igroup.h>
#include <memory>

namespace uff {

class SuperGroup : public IGroup {
 public:
  // CTOR & DTOR
  SuperGroup() = delete;
  SuperGroup(std::weak_ptr<Group> initialGroup, uint32_t repetition_count,
             MetadataType time_offset = 0.)
      : IGroup(repetition_count, time_offset), _initialGroup(std::move(initialGroup)) {}
  SuperGroup(const SuperGroup&) = default;
  SuperGroup(SuperGroup&&) noexcept = default;
  ~SuperGroup() override = default;

  // Operators
  SuperGroup& operator=(const SuperGroup& other) noexcept = default;
  SuperGroup& operator=(SuperGroup&& other) noexcept = default;
  inline bool operator==(const SuperGroup& other) const {
    return _initialGroup.lock() == other._initialGroup.lock();
  }
  inline bool operator!=(const SuperGroup& other) const { return !(*this == other); }

  // Accessors
  inline const std::weak_ptr<Group>& initialGroup() const { return _initialGroup; }
  inline void setInitialGroup(const std::weak_ptr<Group>& initialGroup) {
    _initialGroup = initialGroup;
  }

  // Members
 private:
  std::weak_ptr<Group> _initialGroup;
};

}  // namespace uff
