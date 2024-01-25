#include <algorithm>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <system_error>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/detail/raw_data.h>
#include <urx/utils/common.h>
#include <urx/utils/io/reader_v0_3.h>
#include <urx/v0_3/acquisition.h>
#include <urx/v0_3/aperture.h>
#include <urx/v0_3/dataset.h>
#include <urx/v0_3/element.h>
#include <urx/v0_3/element_geometry.h>
#include <urx/v0_3/event.h>
#include <urx/v0_3/excitation.h>
#include <urx/v0_3/group.h>
#include <urx/v0_3/group_data.h>
#include <urx/v0_3/group_link.h>
#include <urx/v0_3/igroup.h>
#include <urx/v0_3/impulse_response.h>
#include <urx/v0_3/linear_array.h>
#include <urx/v0_3/matrix_array.h>
#include <urx/v0_3/position.h>
#include <urx/v0_3/probe.h>
#include <urx/v0_3/rca_array.h>
#include <urx/v0_3/receive_setup.h>
#include <urx/v0_3/rotation.h>
#include <urx/v0_3/sequence.h>
#include <urx/v0_3/super_group.h>
#include <urx/v0_3/timed_event.h>
#include <urx/v0_3/transform.h>
#include <urx/v0_3/translation.h>
#include <urx/v0_3/transmit_setup.h>
#include <urx/v0_3/trigger_destination.h>
#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>
#include <urx/v0_3/trigger_source.h>
#include <urx/v0_3/version.h>
#include <urx/v0_3/wave.h>

namespace urx::utils::io::v0_3 {

namespace {

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

template <typename T>
const std::vector<std::shared_ptr<T>>& get_shared_ptr(MapToSharedPtr& map) {
  return *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(
      map.at(std::type_index{typeid(T)}));
}

void read(uint16_t& number, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_UINT16);
  dataset.read(&number, datatype);
}

void read(uint32_t& number, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_UINT32);
  dataset.read(&number, datatype);
}

void read(int32_t& number, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_INT32);
  dataset.read(&number, datatype);
}

void read(uint64_t& number, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_UINT64);
  dataset.read(&number, datatype);
}

void read(double& number, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
  dataset.read(&number, datatype);
}

void read(std::string& text, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype = dataset.getStrType();
  const H5::DataSpace dataspace(H5S_SCALAR);
  dataset.read(text, datatype, dataspace);
}

void read(urx::v0_3::Version& version, const H5::Group& group, const std::string& name) {
  const H5::Group group_child(group.openGroup(name));

  read(version.major, group_child, "major");
  read(version.minor, group_child, "minor");
  read(version.patch, group_child, "patch");
  read(version.specialization, group_child, "specialization");
}

void read(urx::v0_3::TriggerDestination& version, const H5::Group& group, const std::string& name) {
  int32_t value;
  read(value, group, name);
  version = static_cast<urx::v0_3::TriggerDestination>(value);
}

template <class T>
std::enable_if_t<std::is_enum_v<T>, void> read(T& version, const H5::Group& group,
                                               const std::string& name, MapToSharedPtr&) {
  int32_t value;
  read(value, group, name);
  version = static_cast<T>(value);
}

void read(std::shared_ptr<urx::v0_3::GroupLink>& igroup, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(urx::v0_3::Sequence& sequence, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::IGroup>& field, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
template <typename T>
void read(std::vector<T>& vector, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::vector<double>& vector, const H5::Group& group, const std::string& name);
void read(std::vector<std::vector<uint32_t>>& vector, const H5::Group& group,
          const std::string& name);
void read(std::optional<urx::v0_3::TriggerIn>& trigger_in, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(std::optional<urx::v0_3::TriggerOut>& trigger_out, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::Event>& event, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::Probe>& probe, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::ElementGeometry>& element_geometry, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(urx::v0_3::Position& position, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::ImpulseResponse>& impulse_response, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(urx::v0_3::Element& element, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::Wave>& wave, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::Excitation>& excitation, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(std::shared_ptr<urx::v0_3::GroupData>& group_data, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map);
void read(urx::v0_3::TransmitSetup& transmit_setup, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(urx::v0_3::ReceiveSetup& receive_setup, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(urx::v0_3::TimedEvent& timed_event, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map);
void read(std::shared_ptr<urx::RawData>& field, const H5::Group& group, const std::string& name);

template <typename T>
void read(std::weak_ptr<T>& weak, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  if (group.nameExists(name)) {
    std::string idx;

    read(idx, group, name);

    if (idx.empty()) {
      return;
    }

    weak = get_shared_ptr<T>(map)[std::stoi(idx) - 1];
  }
}

void read(std::shared_ptr<urx::v0_3::GroupLink>& igroup, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  igroup = std::make_shared<urx::v0_3::GroupLink>();

  read(igroup->trigger_in, group_child, "trigger_in", map);
  read(igroup->source, group_child, "source_id", map);
  read(igroup->destination, group_child, "destination_id", map);
}

void read(urx::v0_3::TimedEvent& timed_event, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(timed_event.time_offset, group_child, "time_offset");
  read(timed_event.event, group_child, "event_id", map);
}

void read(std::shared_ptr<urx::RawData>& field, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::DataSpace dataspace = dataset.getSpace();
  const H5::DataType datatype_raw = dataset.getDataType();

  const int ndims = dataspace.getSimpleExtentNdims();
  std::vector<hsize_t> dimension;
  dimension.resize(ndims);
  dataspace.getSimpleExtentDims(dimension.data());

  field = std::make_shared<RawDataNoInit<int16_t>>(dimension[0] * dimension[1] * dimension[2] *
                                                   dimension[3]);

  dataset.read(field->getBuffer(), datatype_raw);
}

void read(urx::v0_3::Sequence& sequence, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(sequence.time_offset, group_child, "time_offset");
  read(sequence.trigger_in, group_child, "trigger_in", map);
  read(sequence.trigger_out, group_child, "trigger_out", map);
  read(sequence.timed_events, group_child, "timed_events", map);
}

// NOLINTNEXTLINE(misc-no-recursion)
void read(std::shared_ptr<urx::v0_3::IGroup>& field, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  if (group_child.getNumObjs() == 7) {
    const std::shared_ptr<urx::v0_3::Group> new_group = std::make_shared<urx::v0_3::Group>();
    read(new_group->sequence, group_child, "sequence", map);
    read(new_group->repetition_rate, group_child, "repetition_rate");
    field = new_group;
  } else {
    const std::shared_ptr<urx::v0_3::SuperGroup> new_group =
        std::make_shared<urx::v0_3::SuperGroup>();
    read(new_group->initial_group, group_child, "initial_group", map);
    field = new_group;
  }

  read(field->name, group_child, "name");
  read(field->time_offset, group_child, "time_offset");
  read(field->trigger_in, group_child, "trigger_in", map);
  read(field->trigger_out, group_child, "trigger_out", map);
  read(field->repetition_count, group_child, "repetition_count");
}

template <typename T>
void read(std::vector<T>& vector, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  constexpr int iter_length = 8;
  for (size_t i = 0; i < group_child.getNumObjs(); i++) {
    T t;
    read(t, group_child, format_index_with_leading_zeros(i + 1, iter_length), map);
    vector.push_back(std::move(t));
  }
}

template <typename T>
void read(std::vector<std::weak_ptr<T>>& vector, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();

  std::vector<hsize_t> dimension;
  dimension.resize(ndims);
  dataspace.getSimpleExtentDims(dimension.data());

  std::vector<char> raw_data;
  raw_data.resize(dimension[0] * 8);
  dataset.read(raw_data.data(), dataset.getStrType());

  vector.reserve(dimension[0]);

  for (size_t i = 0; i < dimension[0]; i++) {
    uint32_t idx;
    const std::from_chars_result retval =
        std::from_chars(raw_data.data() + (i * 8), raw_data.data() + ((i + 1) * 8), idx);
    if (retval.ec == std::errc{}) {
      vector.push_back(get_shared_ptr<T>(map)[idx - 1]);
    }
  }
}

void read(std::vector<double>& vector, const H5::Group& group, const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();
  std::vector<hsize_t> dimension;
  dimension.resize(ndims);
  dataspace.getSimpleExtentDims(dimension.data());
  vector.resize(dimension[1]);
  dataset.read(vector.data(), datatype);
}

void read(std::vector<urx::v0_3::TriggerSource>& vector, const H5::Group& group,
          const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_INT32);
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();
  std::vector<hsize_t> dimension;
  dimension.resize(ndims);
  dataspace.getSimpleExtentDims(dimension.data());
  vector.resize(dimension[1]);
  dataset.read(vector.data(), datatype);
}

void read(std::vector<std::vector<uint32_t>>& vector, const H5::Group& group,
          const std::string& name) {
  const H5::DataSet dataset = group.openDataSet(name);
  const H5::StrType datatype(H5::PredType::NATIVE_INT32);
  const H5::DataSpace dataspace = dataset.getSpace();
  const int ndims = dataspace.getSimpleExtentNdims();

  std::vector<hsize_t> dimension;
  dimension.resize(ndims);
  dataspace.getSimpleExtentDims(dimension.data());

  std::vector<uint32_t> raw_data;
  raw_data.resize(std::max(dimension[0], dimension[1]));
  dataset.read(raw_data.data(), datatype);

  vector.reserve(raw_data.size());
  std::transform(raw_data.begin(), raw_data.end(), std::back_inserter(vector),
                 [](uint32_t value) { return std::vector<uint32_t>{value}; });
}

void read(std::optional<urx::v0_3::TriggerIn>& trigger_in, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  if (!group_child.nameExists("sources") || !group_child.nameExists("edge")) {
    return;
  }

  trigger_in = urx::v0_3::TriggerIn{};

  read(trigger_in->sources, group_child, "sources");
  read(trigger_in->edge, group_child, "edge", map);
}

void read(urx::v0_3::TransmitSetup& transmit_setup, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(transmit_setup.time_offset, group_child, "time_offset");

  read(transmit_setup.probe, group_child, "probe_id", map);
  read(transmit_setup.wave, group_child, "wave_id", map);
}

void read(urx::v0_3::ReceiveSetup& receive_setup, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(receive_setup.time_offset, group_child, "time_offset");

  read(receive_setup.probe, group_child, "probe_id", map);

  read(receive_setup.sampling_frequency, group_child, "sampling_frequency");
  read(receive_setup.nb_samples, group_child, "nb_samples");
  read(receive_setup.sampling_type, group_child, "sampling_type", map);
  read(receive_setup.channel_mapping, group_child, "channel_mapping");
  read(receive_setup.tgc_profile, group_child, "tgc_profile");
  read(receive_setup.tgc_sampling_frequency, group_child, "tgc_sampling_frequency");
  read(receive_setup.modulation_frequency, group_child, "modulation_frequency");
}

void read(std::shared_ptr<urx::v0_3::Event>& event, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  event = std::make_shared<urx::v0_3::Event>();

  read(event->transmit_setup, group_child, "transmit_setup", map);
  read(event->receive_setup, group_child, "receive_setup", map);

  read(event->trigger_in, group_child, "trigger_in", map);
  read(event->trigger_out, group_child, "trigger_out", map);
}

void read(urx::v0_3::Position& position, const H5::Group& group, const std::string& name,
          MapToSharedPtr& /*map*/) {
  const H5::Group group_child(group.openGroup(name));

  read(position.x, group_child, "x");
  read(position.y, group_child, "y");
  read(position.z, group_child, "z");
}

void read(std::shared_ptr<urx::v0_3::ElementGeometry>& element_geometry, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  element_geometry = std::make_shared<urx::v0_3::ElementGeometry>();

  read(element_geometry->positions, group_child, "positions", map);
}

void read(std::shared_ptr<urx::v0_3::ImpulseResponse>& impulse_response, const H5::Group& group,
          const std::string& name, MapToSharedPtr& /*map*/) {
  const H5::Group group_child(group.openGroup(name));

  impulse_response = std::make_shared<urx::v0_3::ImpulseResponse>();

  read(impulse_response->time_offset, group_child, "time_offset");
  read(impulse_response->sampling_frequency, group_child, "sampling_frequency");
  read(impulse_response->data, group_child, "data");
  read(impulse_response->units, group_child, "units");
}

void read(urx::v0_3::Translation& translation, const H5::Group& group, const std::string& name) {
  const H5::Group group_child(group.openGroup(name));

  read(translation.x, group_child, "x");
  read(translation.y, group_child, "y");
  read(translation.z, group_child, "z");
}

void read(urx::v0_3::Rotation& rotation, const H5::Group& group, const std::string& name) {
  const H5::Group group_child(group.openGroup(name));

  read(rotation.x, group_child, "x");
  read(rotation.y, group_child, "y");
  read(rotation.z, group_child, "z");
}

void read(urx::v0_3::Transform& transform, const H5::Group& group, const std::string& name) {
  const H5::Group group_child(group.openGroup(name));

  read(transform.translation, group_child, "translation");
  read(transform.rotation, group_child, "rotation");
}

void read(urx::v0_3::Element& element, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(element.transform, group_child, "transform");

  read(element.element_geometry, group_child, "element_geometry_id", map);
  read(element.impulse_response, group_child, "impulse_response_id", map);
}

void read(std::optional<urx::v0_3::Aperture>& aperture, const H5::Group& group,
          const std::string& name) {
  const H5::Group group_child(group.openGroup(name));

  if (!group_child.nameExists("origin") || !group_child.nameExists("window") ||
      !group_child.nameExists("f_number") || !group_child.nameExists("fixed_size")) {
    return;
  }

  aperture = urx::v0_3::Aperture{};

  read(aperture->origin, group_child, "origin");
  read(aperture->window, group_child, "window");
  read(aperture->f_number, group_child, "f_number");
  read(aperture->fixed_size, group_child, "fixed_size");
}

void read(std::shared_ptr<urx::v0_3::Probe>& probe, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  std::string probe_type;

  read(probe_type, group_child, "probe_type");

  if (probe_type == "LinearArray") {
    const std::shared_ptr<urx::v0_3::LinearArray> new_probe =
        std::make_shared<urx::v0_3::LinearArray>();

    read(new_probe->number_elements, group_child, "number_elements");
    read(new_probe->pitch, group_child, "pitch");
    read(new_probe->element_width, group_child, "element_width");
    read(new_probe->element_height, group_child, "element_height");

    probe = new_probe;
  } else if (probe_type == "MatrixArray") {
    const std::shared_ptr<urx::v0_3::MatrixArray> new_probe =
        std::make_shared<urx::v0_3::MatrixArray>();

    read(new_probe->number_elements_x, group_child, "number_elements_x");
    read(new_probe->number_elements_y, group_child, "number_elements_y");
    read(new_probe->pitch_x, group_child, "pitch_x");
    read(new_probe->pitch_y, group_child, "pitch_y");
    read(new_probe->element_width, group_child, "element_width");
    read(new_probe->element_height, group_child, "element_height");

    probe = new_probe;
  } else if (probe_type == "RcaArray") {
    const std::shared_ptr<urx::v0_3::RcaArray> new_probe = std::make_shared<urx::v0_3::RcaArray>();

    read(new_probe->number_elements_x, group_child, "number_elements_x");
    read(new_probe->number_elements_y, group_child, "number_elements_y");
    read(new_probe->pitch_x, group_child, "pitch_x");
    read(new_probe->pitch_y, group_child, "pitch_y");
    read(new_probe->element_width_x, group_child, "element_width_x");
    read(new_probe->element_width_y, group_child, "element_width_y");
    read(new_probe->element_height_x, group_child, "element_height_x");
    read(new_probe->element_height_y, group_child, "element_height_y");

    probe = new_probe;
  }

  read(probe->transform, group_child, "transform");
  read(probe->focal_length, group_child, "focal_length");
  read(probe->element_geometries, group_child, "element_geometries", map);
  read(probe->impulse_responses, group_child, "impulse_responses", map);
  map.insert({typeid(urx::v0_3::ElementGeometry), &probe->element_geometries});
  map.insert({typeid(urx::v0_3::ImpulseResponse), &probe->impulse_responses});
  read(probe->elements, group_child, "elements", map);
  map.erase(typeid(urx::v0_3::ElementGeometry));
  map.erase(typeid(urx::v0_3::ImpulseResponse));
}

void read(std::shared_ptr<urx::v0_3::Wave>& wave, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  wave = std::make_shared<urx::v0_3::Wave>();

  read(wave->origin, group_child, "origin");
  read(wave->wave_type, group_child, "wave_type", map);
  read(wave->aperture, group_child, "aperture");
  read(wave->time_zero_reference_point, group_child, "time_zero_reference_point", map);
  read(wave->time_zero, group_child, "time_zero");

  read(wave->channel_mapping, group_child, "channel_mapping");
  read(wave->channel_excitations, group_child, "channel_excitations_id", map);
  read(wave->channel_delays, group_child, "channel_delays");
}

void read(std::shared_ptr<urx::v0_3::Excitation>& excitation, const H5::Group& group,
          const std::string& name, MapToSharedPtr& /*map*/) {
  const H5::Group group_child(group.openGroup(name));

  excitation = std::make_shared<urx::v0_3::Excitation>();

  read(excitation->pulse_shape, group_child, "pulse_shape");
  read(excitation->transmit_frequency, group_child, "transmit_frequency");
  read(excitation->sampling_frequency, group_child, "sampling_frequency");
  read(excitation->waveform, group_child, "waveform");
}

void read(std::shared_ptr<urx::v0_3::GroupData>& group_data, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  group_data = std::make_shared<urx::v0_3::GroupData>();

  read(group_data->group, group_child, "group_id", map);
  read(group_data->data, group_child, "data");
  read(group_data->timestamp, group_child, "timestamp");
  //read(group_data->sequence_timestamps, group_child, "sequence_timestamps");
  //read(group_data->event_timestamps, group_child, "event_timestamps", map);
}

void read(std::optional<urx::v0_3::TriggerOut>& trigger_out, const H5::Group& group,
          const std::string& name, MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  if (!group_child.nameExists("time_offset") || !group_child.nameExists("pulse_duration") ||
      !group_child.nameExists("destination") || !group_child.nameExists("polarity")) {
    return;
  }

  trigger_out = urx::v0_3::TriggerOut{};

  read(trigger_out->time_offset, group_child, "time_offset");
  read(trigger_out->pulse_duration, group_child, "pulse_duration");
  read(trigger_out->destination, group_child, "destination");
  read(trigger_out->polarity, group_child, "polarity", map);
}

void read(urx::v0_3::Acquisition& acq, const H5::Group& group, const std::string& name,
          MapToSharedPtr& map) {
  const H5::Group group_child(group.openGroup(name));

  read(acq.time_offset, group_child, "time_offset");

  read(acq.authors, group_child, "authors");
  read(acq.description, group_child, "description");
  read(acq.local_time, group_child, "local_time");
  read(acq.country_code, group_child, "country_code");
  read(acq.system, group_child, "system");
  read(acq.sound_speed, group_child, "sound_speed");
  read(acq.timestamp, group_child, "timestamp");

  read(acq.trigger_in, group_child, "trigger_in", map);
  read(acq.trigger_out, group_child, "trigger_out", map);

  read(acq.excitation, group_child, "excitations", map);
  read(acq.probes, group_child, "probes", map);
  read(acq.waves, group_child, "waves", map);
  read(acq.unique_events, group_child, "unique_events", map);
  read(acq.groups, group_child, "groups", map);
  read(acq.group_link, group_child, "group_links", map);

  read(acq.initial_group, group_child, "initial_group_id", map);

  read(acq.group_data, group_child, "group_data", map);
}

}  // namespace

std::shared_ptr<urx::v0_3::Dataset> Reader::loadFromFile(const std::string& filename) {
  auto dataset = std::make_shared<urx::v0_3::Dataset>();

  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);
  MapToSharedPtr map_to_shared_ptr{
      {typeid(urx::v0_3::GroupLink), &dataset->acquisition.group_link},
      {typeid(urx::v0_3::IGroup), &dataset->acquisition.groups},
      {typeid(urx::v0_3::Probe), &dataset->acquisition.probes},
      {typeid(urx::v0_3::Event), &dataset->acquisition.unique_events},
      {typeid(urx::v0_3::Wave), &dataset->acquisition.waves},
      {typeid(urx::v0_3::Excitation), &dataset->acquisition.excitation},
      {typeid(urx::v0_3::GroupData), &dataset->acquisition.group_data}};

  read(dataset->version, file, "version");
  read(dataset->acquisition, file, "acquisition", map_to_shared_ptr);

  return dataset;
}

}  // namespace urx::utils::io::v0_3
