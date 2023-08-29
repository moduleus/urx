#pragma once

#include <uff/destination_link.h>
#include <uff/group.h>
#include <uff/igroup.h>
#include <memory>

namespace uff {

class SuperGroup : public IGroup {
 public:
  // CTOR & DTOR
  SuperGroup() = delete;
  SuperGroup(std::weak_ptr<IGroup> initialGroup, uint32_t repetition_count,
             std::vector<DestinationLink> destination_links = {}, double time_offset = 0.)
      : IGroup(repetition_count, std::move(destination_links), time_offset),
        _initialGroup(std::move(initialGroup)) {}
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
  inline const std::weak_ptr<IGroup>& initialGroup() const { return _initialGroup; }
  inline void setInitialGroup(const std::weak_ptr<IGroup>& initialGroup) {
    _initialGroup = initialGroup;
  }

  // Members
 private:
  std::weak_ptr<IGroup> _initialGroup;
};

}  // namespace uff
