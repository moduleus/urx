#include <memory>

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

std::unordered_map<std::type_index, std::unordered_map<size_t, std::string>>
    SerializeHelper::member_name{
        {typeid(Acquisition),
         {MAP_ROW(Acquisition, authors), MAP_ROW(Acquisition, description),
          MAP_ROW(Acquisition, local_time), MAP_ROW(Acquisition, country_code),
          MAP_ROW(Acquisition, system), MAP_ROW(Acquisition, sound_speed),
          MAP_ROW(Acquisition, timestamp), MAP_ROW(Acquisition, probes),
          MAP_ROW(Acquisition, excitations), MAP_ROW(Acquisition, groups),
          MAP_ROW(Acquisition, groups_data)}},
        {typeid(Dataset), {MAP_ROW(Dataset, acquisition), MAP_ROW(Dataset, version)}},
        {typeid(Element),
         {MAP_ROW(Element, transform), MAP_ROW(Element, element_geometry),
          MAP_ROW(Element, impulse_response)}},
        {typeid(ElementGeometry), {MAP_ROW(ElementGeometry, perimeter)}},
        {typeid(Event), {MAP_ROW(Event, transmit_setup), MAP_ROW(Event, receive_setup)}},
        {typeid(Excitation),
         {MAP_ROW(Excitation, pulse_shape), MAP_ROW(Excitation, transmit_frequency),
          MAP_ROW(Excitation, sampling_frequency), MAP_ROW(Excitation, waveform)}},
        {typeid(Group),
         {MAP_ROW(Group, sampling_type), MAP_ROW(Group, data_type), MAP_ROW(Group, description),
          MAP_ROW(Group, sequence)}},
        {typeid(GroupData),
         {MAP_ROW(GroupData, group), MAP_ROW(GroupData, raw_data),
          MAP_ROW(GroupData, group_timestamp), MAP_ROW(GroupData, sequence_timestamps),
          MAP_ROW(GroupData, event_timestamps)}},
        {typeid(ImpulseResponse),
         {MAP_ROW(ImpulseResponse, sampling_frequency), MAP_ROW(ImpulseResponse, time_offset),
          MAP_ROW(ImpulseResponse, units), MAP_ROW(ImpulseResponse, data)}},
        {typeid(Probe),
         {MAP_ROW(Probe, description), MAP_ROW(Probe, type), MAP_ROW(Probe, transform),
          MAP_ROW(Probe, element_geometries), MAP_ROW(Probe, impulse_responses),
          MAP_ROW(Probe, elements)}},
        {typeid(ReceiveSetup),
         {MAP_ROW(ReceiveSetup, probe), MAP_ROW(ReceiveSetup, probe_transform),
          MAP_ROW(ReceiveSetup, sampling_frequency), MAP_ROW(ReceiveSetup, number_samples),
          MAP_ROW(ReceiveSetup, active_elements), MAP_ROW(ReceiveSetup, tgc_profile),
          MAP_ROW(ReceiveSetup, tgc_sampling_frequency),
          MAP_ROW(ReceiveSetup, modulation_frequency), MAP_ROW(ReceiveSetup, time_offset)}},
        {typeid(Transform), {MAP_ROW(Transform, rotation), MAP_ROW(Transform, translation)}},
        {typeid(TransmitSetup),
         {MAP_ROW(TransmitSetup, probe), MAP_ROW(TransmitSetup, wave),
          MAP_ROW(TransmitSetup, active_elements), MAP_ROW(TransmitSetup, excitations),
          MAP_ROW(TransmitSetup, delays), MAP_ROW(TransmitSetup, probe_transform),
          MAP_ROW(TransmitSetup, time_offset)}},
        {typeid(Vector3D<double>),
         {MAP_ROW(Vector3D<double>, x), MAP_ROW(Vector3D<double>, y),
          MAP_ROW(Vector3D<double>, z)}},
        {typeid(Version),
         {MAP_ROW(Version, major), MAP_ROW(Version, minor), MAP_ROW(Version, patch)}},
        {typeid(Wave),
         {MAP_ROW(Wave, type), MAP_ROW(Wave, time_zero), MAP_ROW(Wave, time_zero_reference_point),
          MAP_ROW(Wave, parameters)}},
    };

}  // namespace urx::utils::io
