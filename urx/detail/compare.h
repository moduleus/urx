#pragma once

#include <memory>

namespace urx {

struct ElementGeometry;
struct Excitation;
struct ReceiveSetup;
struct ImpulseResponse;
struct Probe;
struct Wave;
class RawData;

namespace detail {
template <class T>
struct TransmitSetup;
template <class TS, class RS>
struct Event;
template <class T>
struct Group;
template <class T>
struct GroupData;
}  // namespace detail

using TransmitSetup = detail::TransmitSetup<Excitation>;
using Event = detail::Event<TransmitSetup, ReceiveSetup>;
using Group = detail::Group<Event>;
using GroupData = detail::GroupData<Group>;

template <typename T>
inline std::enable_if_t<std::is_same_v<ElementGeometry, T> || std::is_same_v<Excitation, T> ||
                            std::is_same_v<Group, T> || std::is_same_v<GroupData, T> ||
                            std::is_same_v<ImpulseResponse, T> || std::is_same_v<Probe, T> ||
                            std::is_same_v<RawData, T> || std::is_same_v<Wave, T>,
                        bool>
operator==(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) {
  auto lhs_lock = lhs.lock();
  auto rhs_lock = rhs.lock();

  return (lhs_lock && rhs_lock) ? (*lhs_lock == *rhs_lock) : (!!lhs_lock == !!rhs_lock);
}

template <typename T>
inline std::enable_if_t<std::is_same_v<ElementGeometry, T> || std::is_same_v<Excitation, T> ||
                            std::is_same_v<Group, T> || std::is_same_v<GroupData, T> ||
                            std::is_same_v<ImpulseResponse, T> || std::is_same_v<Probe, T> ||
                            std::is_same_v<RawData, T> || std::is_same_v<Wave, T>,
                        bool>
operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) {
  return (lhs && rhs) ? (*lhs == *rhs) : (!!lhs == !!rhs);
}

}  // namespace urx