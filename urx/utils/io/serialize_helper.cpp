#include <functional>
#include <memory>
#include <utility>

#include <urx/acquisition.h>
#include <urx/dataset.h>
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
#define INDEXOF(classe, member) reinterpret_cast<size_t>(&reinterpret_cast<classe*>(0)->member)

#define MAP_ROW(classe, member) \
  { INDEXOF(classe, member), str(member) }

std::map<std::type_index, std::map<size_t, std::string>> SerializeHelper::member_name{
    {typeid(urx::Acquisition),
     {MAP_ROW(urx::Acquisition, authors), MAP_ROW(urx::Acquisition, description),
      MAP_ROW(urx::Acquisition, local_time), MAP_ROW(urx::Acquisition, country_code),
      MAP_ROW(urx::Acquisition, system), MAP_ROW(urx::Acquisition, sound_speed),
      MAP_ROW(urx::Acquisition, timestamp), MAP_ROW(urx::Acquisition, probes),
      MAP_ROW(urx::Acquisition, excitations), MAP_ROW(urx::Acquisition, waves),
      MAP_ROW(urx::Acquisition, groups), MAP_ROW(urx::Acquisition, groups_data)}},
    {typeid(urx::Dataset), {MAP_ROW(urx::Dataset, acquisition), MAP_ROW(urx::Dataset, version)}},
    {typeid(urx::Element),
     {MAP_ROW(urx::Element, transform), MAP_ROW(urx::Element, element_geometry),
      MAP_ROW(urx::Element, impulse_response)}},
    {typeid(urx::ElementGeometry), {MAP_ROW(urx::ElementGeometry, perimeter)}},
    {typeid(urx::Event), {MAP_ROW(urx::Event, transmit_setup), MAP_ROW(urx::Event, receive_setup)}},
    {typeid(urx::Excitation),
     {MAP_ROW(urx::Excitation, pulse_shape), MAP_ROW(urx::Excitation, transmit_frequency),
      MAP_ROW(urx::Excitation, sampling_frequency), MAP_ROW(urx::Excitation, waveform)}},
    {typeid(urx::Group),
     {MAP_ROW(urx::Group, sampling_type), MAP_ROW(urx::Group, data_type),
      MAP_ROW(urx::Group, description), MAP_ROW(urx::Group, sequence)}},
    {typeid(urx::GroupData),
     {MAP_ROW(urx::GroupData, group), MAP_ROW(urx::GroupData, raw_data),
      MAP_ROW(urx::GroupData, group_timestamp), MAP_ROW(urx::GroupData, sequence_timestamps),
      MAP_ROW(urx::GroupData, event_timestamps)}},
    {typeid(urx::ImpulseResponse),
     {MAP_ROW(urx::ImpulseResponse, sampling_frequency), MAP_ROW(urx::ImpulseResponse, time_offset),
      MAP_ROW(urx::ImpulseResponse, units), MAP_ROW(urx::ImpulseResponse, data)}},
    {typeid(urx::Probe),
     {MAP_ROW(urx::Probe, description), MAP_ROW(urx::Probe, type), MAP_ROW(urx::Probe, transform),
      MAP_ROW(urx::Probe, element_geometries), MAP_ROW(urx::Probe, impulse_responses),
      MAP_ROW(urx::Probe, elements)}},
    {typeid(urx::ReceiveSetup),
     {MAP_ROW(urx::ReceiveSetup, probe), MAP_ROW(urx::ReceiveSetup, probe_transform),
      MAP_ROW(urx::ReceiveSetup, sampling_frequency), MAP_ROW(urx::ReceiveSetup, number_samples),
      MAP_ROW(urx::ReceiveSetup, channel_mapping), MAP_ROW(urx::ReceiveSetup, tgc_profile),
      MAP_ROW(urx::ReceiveSetup, tgc_sampling_frequency),
      MAP_ROW(urx::ReceiveSetup, modulation_frequency), MAP_ROW(urx::ReceiveSetup, time_offset)}},
    {typeid(urx::Transform),
     {MAP_ROW(urx::Transform, rotation), MAP_ROW(urx::Transform, translation)}},
    {typeid(urx::TransmitSetup),
     {MAP_ROW(urx::TransmitSetup, probe), MAP_ROW(urx::TransmitSetup, wave),
      MAP_ROW(urx::TransmitSetup, probe_transform), MAP_ROW(urx::TransmitSetup, time_offset)}},
    {typeid(urx::Vector3D<double>),
     {MAP_ROW(urx::Vector3D<double>, x), MAP_ROW(urx::Vector3D<double>, y),
      MAP_ROW(urx::Vector3D<double>, z)}},
    {typeid(urx::Version),
     {MAP_ROW(urx::Version, major), MAP_ROW(urx::Version, minor), MAP_ROW(urx::Version, patch)}},
    {typeid(urx::Wave),
     {MAP_ROW(urx::Wave, type), MAP_ROW(urx::Wave, time_zero),
      MAP_ROW(urx::Wave, time_zero_reference_point), MAP_ROW(urx::Wave, channel_mapping),
      MAP_ROW(urx::Wave, channel_excitations), MAP_ROW(urx::Wave, channel_delays),
      MAP_ROW(urx::Wave, parameters)}},
};

}  // namespace urx::utils::io
