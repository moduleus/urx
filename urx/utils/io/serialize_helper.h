#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/utils/export.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::io {

using AllTypeInVariant = std::variant<
    Acquisition*, DataType*, DoubleNan*, ProbeType*, ReceiveSetup*, SamplingType*, Transform*,
    TransmitSetup*, Vector3D<double>*, Version*, Wave*, WaveType*, double*, float*, int16_t*,
    int32_t*, int64_t*, int8_t*, std::shared_ptr<RawData>*, std::string*, std::vector<Element>*,
    std::vector<Event>*, std::vector<GroupData>*, std::vector<Vector3D<double>>*,
    std::vector<double>*, std::vector<float>*, std::vector<int16_t>*, std::vector<int32_t>*,
    std::vector<int64_t>*, std::vector<int8_t>*, std::vector<std::shared_ptr<ElementGeometry>>*,
    std::vector<std::shared_ptr<Excitation>>*, std::vector<std::shared_ptr<Group>>*,
    std::vector<std::shared_ptr<ImpulseResponse>>*, std::vector<std::shared_ptr<Probe>>*,
    std::vector<std::vector<double>>*, std::vector<std::vector<uint32_t>>*,
    std::vector<std::weak_ptr<Excitation>>*, std::vector<uint16_t>*, std::vector<uint32_t>*,
    std::vector<uint64_t>*, std::vector<uint8_t>*, std::weak_ptr<ElementGeometry>*,
    std::weak_ptr<Group>*, std::weak_ptr<ImpulseResponse>*, std::weak_ptr<Probe>*, uint16_t*,
    uint32_t*, uint64_t*, uint8_t*>;

URX_UTILS_EXPORT const
    std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>&
    getMemberMap();

URX_UTILS_EXPORT const std::unordered_map<std::type_index, const H5::PredType*>& getStdToHdf5();

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

template <typename T>
std::type_index nameTypeid() {
  return typeid(T);
}

constexpr int ITER_LENGTH = 8;
constexpr bool USE_ATTRIBUTE = false;

template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template <typename T>
const std::vector<std::shared_ptr<T>>& getSharedPtr(const MapToSharedPtr& map) {
  return *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(map.at(nameTypeid<T>()));
}

}  // namespace urx::utils::io
