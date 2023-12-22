#pragma once

#include <memory>

namespace urx {

struct ElementGeometry;
struct Excitation;
struct Group;
struct GroupData;
struct ImpulseResponse;
struct Probe;
struct Wave;

template <typename T>
inline auto operator==(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs)
    -> std::enable_if_t<std::is_same_v<ElementGeometry, T> || std::is_same_v<Excitation, T> ||
                            std::is_same_v<Group, T> || std::is_same_v<GroupData, T> ||
                            std::is_same_v<ImpulseResponse, T> || std::is_same_v<Probe, T> ||
                            std::is_same_v<Wave, T>,
                        bool> {
  auto lhs_lock = lhs.lock();
  auto rhs_lock = rhs.lock();

  return (lhs_lock && rhs_lock) ? (*lhs_lock == *rhs_lock) : (!!lhs_lock == !!rhs_lock);
}

template <typename T>
inline auto operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
    -> std::enable_if_t<std::is_same_v<ElementGeometry, T> || std::is_same_v<Excitation, T> ||
                            std::is_same_v<Group, T> || std::is_same_v<GroupData, T> ||
                            std::is_same_v<ImpulseResponse, T> || std::is_same_v<Probe, T> ||
                            std::is_same_v<Wave, T>,
                        bool> {
  return (lhs && rhs) ? (*lhs == *rhs) : (!!lhs == !!rhs);
}

}  // namespace urx