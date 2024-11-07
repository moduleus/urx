#include <memory>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::io {

#define str(s) #s

// NOLINTNEXTLINE(bugprone-macro-parentheses)
#define INDEXOF(c, member) reinterpret_cast<decltype(c::member)*>(&reinterpret_cast<c*>(0)->member)

#define MAP_ROW(c, member) std::make_pair(INDEXOF(c, member), std::string{str(member)})

#if defined __has_attribute
#if __has_attribute(no_sanitize)
__attribute__((no_sanitize("undefined")))
#endif
#endif
const std::unordered_map<std::type_index, std::vector<std::pair<AllTypeInVariant, std::string>>>&
getMemberMap() {
  static const std::unordered_map<std::type_index,
                                  std::vector<std::pair<AllTypeInVariant, std::string>>>
      member_map{
          {nameTypeid<Acquisition>(),
           {MAP_ROW(Acquisition, authors), MAP_ROW(Acquisition, description),
            MAP_ROW(Acquisition, local_time), MAP_ROW(Acquisition, country_code),
            MAP_ROW(Acquisition, system), MAP_ROW(Acquisition, timestamp),
            MAP_ROW(Acquisition, probes), MAP_ROW(Acquisition, excitations),
            MAP_ROW(Acquisition, groups), MAP_ROW(Acquisition, groups_data)}},
          {nameTypeid<Dataset>(), {MAP_ROW(Dataset, acquisition), MAP_ROW(Dataset, version)}},
          {nameTypeid<Element>(),
           {MAP_ROW(Element, transform), MAP_ROW(Element, element_geometry),
            MAP_ROW(Element, impulse_response)}},
          {nameTypeid<ElementGeometry>(), {MAP_ROW(ElementGeometry, perimeter)}},
          {nameTypeid<Event>(), {MAP_ROW(Event, transmit_setup), MAP_ROW(Event, receive_setup)}},
          {nameTypeid<Excitation>(),
           {MAP_ROW(Excitation, pulse_shape), MAP_ROW(Excitation, transmit_frequency),
            MAP_ROW(Excitation, sampling_frequency), MAP_ROW(Excitation, waveform)}},
          {nameTypeid<Group>(),
           {MAP_ROW(Group, sampling_type), MAP_ROW(Group, data_type), MAP_ROW(Group, description),
            MAP_ROW(Group, sound_speed), MAP_ROW(Group, sequence)}},
          {nameTypeid<GroupData>(),
           {MAP_ROW(GroupData, group), MAP_ROW(GroupData, raw_data),
            MAP_ROW(GroupData, group_timestamp), MAP_ROW(GroupData, sequence_timestamps),
            MAP_ROW(GroupData, event_timestamps)}},
          {nameTypeid<ImpulseResponse>(),
           {MAP_ROW(ImpulseResponse, sampling_frequency), MAP_ROW(ImpulseResponse, time_offset),
            MAP_ROW(ImpulseResponse, units), MAP_ROW(ImpulseResponse, data)}},
          {nameTypeid<Probe>(),
           {MAP_ROW(Probe, description), MAP_ROW(Probe, type), MAP_ROW(Probe, transform),
            MAP_ROW(Probe, element_geometries), MAP_ROW(Probe, impulse_responses),
            MAP_ROW(Probe, elements)}},
          {nameTypeid<ReceiveSetup>(),
           {MAP_ROW(ReceiveSetup, probe), MAP_ROW(ReceiveSetup, probe_transform),
            MAP_ROW(ReceiveSetup, sampling_frequency), MAP_ROW(ReceiveSetup, number_samples),
            MAP_ROW(ReceiveSetup, active_elements), MAP_ROW(ReceiveSetup, tgc_profile),
            MAP_ROW(ReceiveSetup, tgc_sampling_frequency),
            MAP_ROW(ReceiveSetup, modulation_frequency), MAP_ROW(ReceiveSetup, time_offset)}},
          {nameTypeid<Transform>(),
           {MAP_ROW(Transform, rotation), MAP_ROW(Transform, translation)}},
          {nameTypeid<TransmitSetup>(),
           {MAP_ROW(TransmitSetup, probe), MAP_ROW(TransmitSetup, wave),
            MAP_ROW(TransmitSetup, active_elements), MAP_ROW(TransmitSetup, excitations),
            MAP_ROW(TransmitSetup, delays), MAP_ROW(TransmitSetup, probe_transform),
            MAP_ROW(TransmitSetup, time_offset)}},
          {nameTypeid<Vector3D<double>>(),
           {MAP_ROW(Vector3D<double>, x), MAP_ROW(Vector3D<double>, y),
            MAP_ROW(Vector3D<double>, z)}},
          {nameTypeid<Version>(),
           {MAP_ROW(Version, major), MAP_ROW(Version, minor), MAP_ROW(Version, patch)}},
          {nameTypeid<Wave>(),
           {MAP_ROW(Wave, type), MAP_ROW(Wave, time_zero), MAP_ROW(Wave, time_zero_reference_point),
            MAP_ROW(Wave, parameters)}},
      };
  return member_map;
}
const std::unordered_map<std::type_index, const H5::PredType*>& getStdToHdf5() {
  static const std::unordered_map<std::type_index, const H5::PredType*> std_to_hdf5{
      {nameTypeid<float>(), &H5::PredType::NATIVE_FLOAT},
      {nameTypeid<double>(), &H5::PredType::NATIVE_DOUBLE},
      {nameTypeid<std::int8_t>(), &H5::PredType::NATIVE_INT8},
      {nameTypeid<std::uint8_t>(), &H5::PredType::NATIVE_UINT8},
      {nameTypeid<std::int16_t>(), &H5::PredType::NATIVE_INT16},
      {nameTypeid<std::uint16_t>(), &H5::PredType::NATIVE_UINT16},
      {nameTypeid<std::int32_t>(), &H5::PredType::NATIVE_INT32},
      {nameTypeid<std::uint32_t>(), &H5::PredType::NATIVE_UINT32},
      {nameTypeid<std::int64_t>(), &H5::PredType::NATIVE_INT64},
      {nameTypeid<std::uint64_t>(), &H5::PredType::NATIVE_UINT64}};
  return std_to_hdf5;
}

}  // namespace urx::utils::io
