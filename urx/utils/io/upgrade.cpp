#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <version>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
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
#include <urx/urx.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_v0_3.h>
#include <urx/utils/io/upgrade.h>
#include <urx/v0_2/dataset.h>
#include <urx/v0_2/linear_array.h>
#include <urx/v0_2/matrix_array.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/rca_array.h>
#include <urx/v0_2/reader.h>
#include <urx/v0_2/types.h>
#include <urx/v0_2/urx.h>
#include <urx/v0_3/acquisition.h>
#include <urx/v0_3/dataset.h>
#include <urx/v0_3/element.h>
#include <urx/v0_3/element_geometry.h>
#include <urx/v0_3/event.h>
#include <urx/v0_3/excitation.h>
#include <urx/v0_3/group.h>
#include <urx/v0_3/group_data.h>
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
#include <urx/v0_3/timed_event.h>
#include <urx/v0_3/transform.h>
#include <urx/v0_3/translation.h>
#include <urx/v0_3/transmit_setup.h>
#include <urx/v0_3/urx.h>
#include <urx/v0_3/wave.h>
#include <urx/v0_3/wave_type.h>
#include <urx/vector.h>
#include <urx/wave.h>

namespace urx::utils::io {

namespace {

///
/// Utility function to correct polarization on samples received on rows elements on RCA.
///
template <class T>
void negateSamplesReceivedOnRows(urx::GroupData& group_data) {
  const urx::Group& group = *group_data.group.lock();
  GroupDataReader data_reader{group_data};
  for (size_t evt_idx = 0; evt_idx < data_reader.eventsCount(); evt_idx++) {
    const auto& event = group.sequence[evt_idx];
    if (event.receive_setup.probe.lock()->type != urx::Probe::ProbeType::RCA) continue;

    const auto& cm = event.receive_setup.active_elements;
    const auto& pe = event.receive_setup.probe.lock()->elements;
    const auto& e0p = pe[cm[0][0]].transform.translation;
    const auto& e1p = pe[cm[1][0]].transform.translation;

    const bool is_received_on_rows = std::abs(e1p.x - e0p.x) > std::abs(e1p.y - e0p.y);

    if (is_received_on_rows) {
      for (size_t seq_idx = 0; seq_idx < data_reader.sequencesCount(); seq_idx++) {
        std::transform(data_reader.operator()<T>(seq_idx, evt_idx),
                       data_reader.operator()<T>(seq_idx, evt_idx + 1),
                       data_reader.operator()<T>(seq_idx, evt_idx), std::negate<T>());
      }
    }
  }
}

// Memo about conversion:
// From uff v0_2, has been removed: Aperture, Probe::focalLength, TransmitWave::weight
template <typename T>
std::shared_ptr<urx::Dataset> ConvertV0_2(const std::string& filename) {
  static std::unordered_map<urx::v0_2::WaveType, urx::Wave::WaveType> old_to_new_wave_type{
      {urx::v0_2::WaveType::CONVERGING_WAVE, urx::Wave::WaveType::CONVERGING_WAVE},
      {urx::v0_2::WaveType::DIVERGING_WAVE, urx::Wave::WaveType::DIVERGING_WAVE},
      {urx::v0_2::WaveType::PLANE_WAVE, urx::Wave::WaveType::PLANE_WAVE},
      {urx::v0_2::WaveType::CYLINDRICAL_WAVE, urx::Wave::WaveType::CYLINDRICAL_WAVE}};

  urx::v0_2::Reader<T> reader;
  reader.setFileName(filename);
  reader.updateMetadata();
  const std::shared_ptr<urx::v0_2::Dataset<T>> dataset_v0_2 = reader.dataset();

  std::shared_ptr<urx::Dataset> retval = std::make_shared<urx::Dataset>();

  retval->acquisition.authors = dataset_v0_2->channelData().authors();
  retval->acquisition.description = dataset_v0_2->channelData().description();
  retval->acquisition.local_time = dataset_v0_2->channelData().localTime();
  retval->acquisition.country_code = dataset_v0_2->channelData().countryCode();
  retval->acquisition.system = dataset_v0_2->channelData().system();
  retval->acquisition.sound_speed = dataset_v0_2->channelData().soundSpeed();
  retval->acquisition.timestamp = 0.;

  for (const auto& old_probe : dataset_v0_2->channelData().probes()) {
    const std::shared_ptr<urx::Probe> new_probe = std::make_shared<urx::Probe>();
    new_probe->description = "";

    if (std::dynamic_pointer_cast<urx::v0_2::LinearArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::LINEAR;
    } else if (std::dynamic_pointer_cast<urx::v0_2::MatrixArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::MATRIX;
    } else if (std::dynamic_pointer_cast<urx::v0_2::RcaArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::RCA;
    } else {
      new_probe->type = urx::Probe::ProbeType::UNDEFINED;
    }

    new_probe->transform.rotation.x = old_probe->transform().rotation().x();
    new_probe->transform.rotation.y = old_probe->transform().rotation().y();
    new_probe->transform.rotation.z = old_probe->transform().rotation().z();
    new_probe->transform.translation.x = old_probe->transform().translation().x();
    new_probe->transform.translation.y = old_probe->transform().translation().y();
    new_probe->transform.translation.z = old_probe->transform().translation().z();

    for (const auto& old_impulse_response : old_probe->impulseResponses()) {
      const std::shared_ptr<urx::ImpulseResponse> new_impulse_response =
          std::make_shared<urx::ImpulseResponse>();

      auto opt_sampling = old_impulse_response.samplingFrequency();
      new_impulse_response->sampling_frequency =
          opt_sampling.has_value() ? *opt_sampling : urx::DoubleNan::NaN;
      std::transform(old_impulse_response.data().begin(), old_impulse_response.data().end(),
                     std::back_inserter(new_impulse_response->data),
                     [](urx::v0_2::MetadataType value) -> double { return value; });
      new_impulse_response->units = old_impulse_response.units();
      new_impulse_response->time_offset = old_impulse_response.initialTime();

      new_probe->impulse_responses.push_back(new_impulse_response);
    }

    new_probe->elements.reserve(old_probe->elements().size());
    size_t element_i = 0;
    for (const auto& old_element : old_probe->elements()) {
      const auto opt_x = old_element.x();
      const auto opt_y = old_element.y();
      const auto opt_z = old_element.z();
      const double x = opt_x.has_value() ? *opt_x : urx::DoubleNan::NaN;
      const double y = opt_y.has_value() ? *opt_y : urx::DoubleNan::NaN;
      const double z = opt_z.has_value() ? *opt_z : urx::DoubleNan::NaN;

      const std::shared_ptr<urx::ElementGeometry> new_element_geometry =
          std::make_shared<urx::ElementGeometry>();
      if (auto old_probe_linear = std::static_pointer_cast<urx::v0_2::LinearArray>(old_probe);
          old_probe_linear) {
        const auto opt_height = old_element.x();
        const double height = opt_height.has_value() ? *opt_height : urx::DoubleNan::NaN;
        new_element_geometry->perimeter.push_back(
            {x - old_probe_linear->pitch() / 2, y - height / 2, z});
        new_element_geometry->perimeter.push_back(
            {x - old_probe_linear->pitch() / 2, y + height / 2, z});
        new_element_geometry->perimeter.push_back(
            {x + old_probe_linear->pitch() / 2, y + height / 2, z});
        new_element_geometry->perimeter.push_back(
            {x + old_probe_linear->pitch() / 2, y - height / 2, z});
      } else if (auto old_probe_rca = std::static_pointer_cast<urx::v0_2::RcaArray>(old_probe);
                 old_probe_rca) {
        if (element_i < old_probe_rca->numberElementsX()) {
          new_element_geometry->perimeter.push_back(
              {x - old_probe_rca->pitchX() / 2,
               y - old_probe_rca->pitchY() / 2 * old_probe_rca->numberElementsY(), z});
          new_element_geometry->perimeter.push_back(
              {x - old_probe_rca->pitchX() / 2,
               y + old_probe_rca->pitchY() / 2 * old_probe_rca->numberElementsY(), z});
          new_element_geometry->perimeter.push_back(
              {x + old_probe_rca->pitchX() / 2,
               y + old_probe_rca->pitchY() / 2 * old_probe_rca->numberElementsY(), z});
          new_element_geometry->perimeter.push_back(
              {x + old_probe_rca->pitchX() / 2,
               y - old_probe_rca->pitchY() / 2 * old_probe_rca->numberElementsY(), z});
        } else {
          new_element_geometry->perimeter.push_back(
              {x - old_probe_rca->pitchX() / 2 * old_probe_rca->numberElementsX(),
               y - old_probe_rca->pitchY() / 2, z});
          new_element_geometry->perimeter.push_back(
              {x - old_probe_rca->pitchX() / 2 * old_probe_rca->numberElementsX(),
               y + old_probe_rca->pitchY() / 2, z});
          new_element_geometry->perimeter.push_back(
              {x + old_probe_rca->pitchX() / 2 * old_probe_rca->numberElementsX(),
               y + old_probe_rca->pitchY() / 2, z});
          new_element_geometry->perimeter.push_back(
              {x + old_probe_rca->pitchX() / 2 * old_probe_rca->numberElementsX(),
               y - old_probe_rca->pitchY() / 2, z});
        }
      } else if (auto old_probe_matrix =
                     std::static_pointer_cast<urx::v0_2::MatrixArray>(old_probe);
                 old_probe_matrix) {
        new_element_geometry->perimeter.push_back(
            {x - old_probe_matrix->pitchX() / 2, y - old_probe_matrix->pitchY() / 2, z});
        new_element_geometry->perimeter.push_back(
            {x - old_probe_matrix->pitchX() / 2, y + old_probe_matrix->pitchY() / 2, z});
        new_element_geometry->perimeter.push_back(
            {x + old_probe_matrix->pitchX() / 2, y + old_probe_matrix->pitchY() / 2, z});
        new_element_geometry->perimeter.push_back(
            {x + old_probe_matrix->pitchX() / 2, y - old_probe_matrix->pitchY() / 2, z});
      }

      new_probe->element_geometries.push_back(new_element_geometry);

      urx::Element new_element;
      new_element.transform.translation = {x, y, z};
      new_element.element_geometry = new_probe->element_geometries.back();
      new_element.impulse_response = element_i < new_probe->impulse_responses.size()
                                         ? new_probe->impulse_responses[element_i]
                                         : nullptr;
      new_probe->elements.push_back(std::move(new_element));

      element_i++;
    }

    retval->acquisition.probes.push_back(new_probe);
  }

  {
    const std::shared_ptr<Group> new_group = std::make_shared<Group>();
    new_group->sampling_type = urx::Group::SamplingType::RF;
    if constexpr (std::is_same_v<T, short>) {
      new_group->data_type = urx::Group::DataType::INT16;
    } else {
      new_group->data_type = urx::Group::DataType::FLOAT;
    }
    new_group->description = "";

    for (const auto& old_sequence : dataset_v0_2->channelData().sequence()) {
      const auto& old_event = old_sequence.evenement().lock();

      Event new_event;
      new_event.transmit_setup.time_offset = 0;
      new_event.transmit_setup.probe_transform = {};
      auto it_probe = std::find_if(dataset_v0_2->channelData().probes().begin(),
                                   dataset_v0_2->channelData().probes().end(),
                                   [old_probe = old_event->transmitSetup().probe().lock()](
                                       const std::shared_ptr<urx::v0_2::Probe>& probe_i) {
                                     return old_probe.get() == probe_i.get();
                                   });
      new_event.transmit_setup.probe =
          retval->acquisition
              .probes[std::distance(dataset_v0_2->channelData().probes().begin(), it_probe)];

      const auto& old_wave = *old_event->transmitSetup().getTransmitWave().wave().lock();
      urx::Wave& new_wave = new_event.transmit_setup.wave;
      new_wave.type = old_to_new_wave_type.find(old_wave.waveType()) != old_to_new_wave_type.end()
                          ? old_to_new_wave_type.at(old_wave.waveType())
                          : urx::Wave::WaveType::UNDEFINED;
      new_wave.time_zero = old_event->transmitSetup().getTransmitWave().timeOffset();
      new_wave.time_zero_reference_point = {0, 0, 0};
      switch (new_wave.type) {
        case urx::Wave::WaveType::CONVERGING_WAVE:
        case urx::Wave::WaveType::DIVERGING_WAVE:
        case urx::Wave::WaveType::PLANE_WAVE: {
          new_wave.parameters = {old_wave.origin().translation().x(),
                                 old_wave.origin().translation().y(),
                                 old_wave.origin().translation().z()};
          break;
        }
        case urx::Wave::WaveType::CYLINDRICAL_WAVE: {
          new_wave.parameters = {
              old_wave.origin().translation().x(), old_wave.origin().translation().y(),
              old_wave.origin().translation().z(), old_wave.origin().rotation().x(),
              old_wave.origin().rotation().y(),    old_wave.origin().rotation().z()};
          break;
        }
        default: {
          break;
        }
      }

      it_probe = std::find_if(dataset_v0_2->channelData().probes().begin(),
                              dataset_v0_2->channelData().probes().end(),
                              [old_probe = old_event->receiveSetup().probe().lock()](
                                  const std::shared_ptr<urx::v0_2::Probe>& probe_i) {
                                return old_probe.get() == probe_i.get();
                              });
      new_event.receive_setup.probe =
          retval->acquisition
              .probes[std::distance(dataset_v0_2->channelData().probes().begin(), it_probe)];
      new_event.receive_setup.time_offset = old_event->receiveSetup().timeOffset();
      new_event.receive_setup.sampling_frequency = old_event->receiveSetup().samplingFrequency();
      std::transform(old_event->receiveSetup().tgcProfile().begin(),
                     old_event->receiveSetup().tgcProfile().end(),
                     std::back_inserter(new_event.receive_setup.tgc_profile),
                     [](urx::v0_2::MetadataType value) -> double { return value; });
      const auto opt_tgc_sampling_frequency = old_event->receiveSetup().tgcSamplingFrequency();
      new_event.receive_setup.tgc_sampling_frequency = opt_tgc_sampling_frequency.has_value()
                                                           ? *opt_tgc_sampling_frequency
                                                           : urx::DoubleNan::NaN;
      const auto opt_modulation_frequency = old_event->receiveSetup().modulationFrequency();
      new_event.receive_setup.modulation_frequency =
          opt_modulation_frequency.has_value() ? *opt_modulation_frequency : urx::DoubleNan::NaN;
      new_event.receive_setup.probe_transform = {};
      new_event.receive_setup.active_elements.reserve(
          old_event->receiveSetup().channelMapping().size());
      std::transform(old_event->receiveSetup().channelMapping().begin(),
                     old_event->receiveSetup().channelMapping().end(),
                     std::back_inserter(new_event.receive_setup.active_elements),
                     [](urx::v0_2::MetadataType value) -> std::vector<uint32_t> {
                       return {static_cast<uint32_t>(value)};
                     });
      new_event.receive_setup.number_samples = dataset_v0_2->channelData().numberOfSamples();

      new_group->sequence.push_back(std::move(new_event));
    }

    retval->acquisition.groups.push_back(new_group);
  }

  {
    GroupData new_group_data;

    new_group_data.group = retval->acquisition.groups[0];

    const size_t sequences = dataset_v0_2->channelData().numberOfFrames();

    new_group_data.raw_data =
        std::make_shared<urx::RawDataVector<T>>(std::move(dataset_v0_2->channelData().data()));

    new_group_data.sequence_timestamps.reserve(sequences);
    new_group_data.event_timestamps.reserve(sequences);
    const auto opt_rate = dataset_v0_2->channelData().repetitionRate();
    const double sequence_rate = opt_rate.has_value() ? *opt_rate : urx::DoubleNan::NaN;
    for (size_t i = 0; i < sequences; i++) {
      new_group_data.sequence_timestamps.push_back(i / sequence_rate);

      std::vector<double> event_offset;
      event_offset.reserve(retval->acquisition.groups[0]->sequence.size());
      for (const auto& event : retval->acquisition.groups[0]->sequence) {
        event_offset.push_back(i / sequence_rate + event.transmit_setup.time_offset);
      }
      new_group_data.event_timestamps.push_back(std::move(event_offset));
    }

    new_group_data.group_timestamp = 0;

    negateSamplesReceivedOnRows<T>(new_group_data);

    retval->acquisition.groups_data.push_back(std::move(new_group_data));
  }

  return retval;
}

std::shared_ptr<urx::Dataset> ConvertV0_3(const std::string& filename) {
  static std::unordered_map<urx::v0_3::WaveType, urx::Wave::WaveType> old_to_new_wave_type{
      {urx::v0_3::WaveType::CONVERGING_WAVE, urx::Wave::WaveType::CONVERGING_WAVE},
      {urx::v0_3::WaveType::DIVERGING_WAVE, urx::Wave::WaveType::DIVERGING_WAVE},
      {urx::v0_3::WaveType::PLANE_WAVE, urx::Wave::WaveType::PLANE_WAVE},
      {urx::v0_3::WaveType::CYLINDRICAL_WAVE, urx::Wave::WaveType::CYLINDRICAL_WAVE}};

  const std::shared_ptr<urx::v0_3::Dataset> dataset_v0_3 =
      urx::utils::io::v0_3::Reader::loadFromFile(filename);

  std::shared_ptr<urx::Dataset> retval = std::make_shared<urx::Dataset>();

  /*
  removed from v0_3:
  Acquisition::time_offset;
  Acquisition::trigger_in;
  Acquisition::trigger_out;
  Acquisition::initial_group;
  Acquisition::group_link;
  Wave::aperture;
*/

  retval->acquisition.authors = dataset_v0_3->acquisition.authors;
  retval->acquisition.description = dataset_v0_3->acquisition.description;
  retval->acquisition.local_time = dataset_v0_3->acquisition.local_time;
  retval->acquisition.country_code = dataset_v0_3->acquisition.country_code;
  retval->acquisition.system = dataset_v0_3->acquisition.system;
  retval->acquisition.sound_speed = dataset_v0_3->acquisition.sound_speed;
  retval->acquisition.timestamp = static_cast<double>(dataset_v0_3->acquisition.timestamp);

  retval->acquisition.excitations.reserve(dataset_v0_3->acquisition.excitation.size());
  for (const auto& old_excitation : dataset_v0_3->acquisition.excitation) {
    const std::shared_ptr<urx::Excitation> new_excitation = std::make_shared<urx::Excitation>();

    new_excitation->pulse_shape = old_excitation->pulse_shape;
    new_excitation->transmit_frequency = old_excitation->transmit_frequency;
    new_excitation->sampling_frequency = old_excitation->sampling_frequency;
    new_excitation->waveform = old_excitation->waveform;

    retval->acquisition.excitations.push_back(new_excitation);
  }

  retval->acquisition.probes.reserve(dataset_v0_3->acquisition.probes.size());
  for (auto& old_probe : dataset_v0_3->acquisition.probes) {
    const std::shared_ptr<urx::Probe> new_probe = std::make_shared<urx::Probe>();
    new_probe->description = "";

    if (std::dynamic_pointer_cast<urx::v0_3::LinearArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::LINEAR;
    } else if (std::dynamic_pointer_cast<urx::v0_3::MatrixArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::MATRIX;
    } else if (std::dynamic_pointer_cast<urx::v0_3::RcaArray>(old_probe)) {
      new_probe->type = urx::Probe::ProbeType::RCA;
    } else {
      new_probe->type = urx::Probe::ProbeType::UNDEFINED;
    }

    new_probe->transform.rotation.x = old_probe->transform.rotation.x;
    new_probe->transform.rotation.y = old_probe->transform.rotation.y;
    new_probe->transform.rotation.z = old_probe->transform.rotation.z;
    new_probe->transform.translation.x = old_probe->transform.translation.x;
    new_probe->transform.translation.y = old_probe->transform.translation.y;
    new_probe->transform.translation.z = old_probe->transform.translation.z;

    new_probe->impulse_responses.reserve(old_probe->impulse_responses.size());
    for (const auto& old_impulse_response : old_probe->impulse_responses) {
      const std::shared_ptr<urx::ImpulseResponse> new_impulse_response =
          std::make_shared<urx::ImpulseResponse>();

      new_impulse_response->time_offset = old_impulse_response->time_offset;
      new_impulse_response->sampling_frequency = old_impulse_response->sampling_frequency;
      new_impulse_response->data = old_impulse_response->data;
      new_impulse_response->units = old_impulse_response->units;

      new_probe->impulse_responses.push_back(new_impulse_response);
    }

    new_probe->element_geometries.reserve(old_probe->element_geometries.size());
    for (const auto& old_element_geometry : old_probe->element_geometries) {
      const std::shared_ptr<urx::ElementGeometry> new_element_geometry =
          std::make_shared<urx::ElementGeometry>();

      std::transform(old_element_geometry->positions.begin(), old_element_geometry->positions.end(),
                     std::back_inserter(new_element_geometry->perimeter),
                     [](const urx::v0_3::Position& value) {
                       return urx::Vector3D<double>{value.x, value.y, value.z};
                     });

      new_probe->element_geometries.push_back(new_element_geometry);
    }

    new_probe->elements.reserve(old_probe->elements.size());
    for (const auto& old_element : old_probe->elements) {
      urx::Element new_element;

      new_element.transform.rotation.x = old_element.transform.rotation.x;
      new_element.transform.rotation.y = old_element.transform.rotation.y;
      new_element.transform.rotation.z = old_element.transform.rotation.z;
      new_element.transform.translation.x = old_element.transform.translation.x;
      new_element.transform.translation.y = old_element.transform.translation.y;
      new_element.transform.translation.z = old_element.transform.translation.z;

      if (auto element_ptr = old_element.element_geometry.lock(); element_ptr) {
        auto it_element = std::find_if(
            old_probe->element_geometries.begin(), old_probe->element_geometries.end(),
            [&element_ptr](const std::shared_ptr<urx::v0_3::ElementGeometry>& elem_geom_i) {
              return element_ptr.get() == elem_geom_i.get();
            });
        new_element.element_geometry = new_probe->element_geometries[std::distance(
            old_probe->element_geometries.begin(), it_element)];
      }

      if (auto imp_ptr = old_element.impulse_response.lock(); imp_ptr) {
        auto it_impulse =
            std::find_if(old_probe->impulse_responses.begin(), old_probe->impulse_responses.end(),
                         [&imp_ptr](const std::shared_ptr<urx::v0_3::ImpulseResponse>& imp_res_i) {
                           return imp_ptr.get() == imp_res_i.get();
                         });
        new_element.impulse_response = new_probe->impulse_responses[std::distance(
            old_probe->impulse_responses.begin(), it_impulse)];
      }

      new_probe->elements.push_back(std::move(new_element));
    }

    retval->acquisition.probes.push_back(new_probe);
  }

  retval->acquisition.groups.reserve(dataset_v0_3->acquisition.groups.size());
  for (const auto& old_group : dataset_v0_3->acquisition.groups) {
    const std::shared_ptr<Group> new_group = std::make_shared<Group>();

    new_group->sampling_type = urx::Group::SamplingType::RF;
    new_group->data_type = urx::Group::DataType::INT16;
    new_group->description = old_group->name;

    if (auto old_real_group = std::dynamic_pointer_cast<urx::v0_3::Group>(old_group);
        old_real_group) {
      for (const auto& old_timed_event : old_real_group->sequence.timed_events) {
        const std::shared_ptr<urx::v0_3::Event> old_event = old_timed_event.event.lock();
        Event new_event;

        new_event.transmit_setup.time_offset = old_event->transmit_setup.time_offset;

        auto it_probe = std::find_if(dataset_v0_3->acquisition.probes.begin(),
                                     dataset_v0_3->acquisition.probes.end(),
                                     [old_probe = old_event->transmit_setup.probe.lock()](
                                         const std::shared_ptr<urx::v0_3::Probe>& probe_i) {
                                       return old_probe.get() == probe_i.get();
                                     });
        new_event.transmit_setup.probe =
            retval->acquisition
                .probes[std::distance(dataset_v0_3->acquisition.probes.begin(), it_probe)];

        auto it_wave = std::find_if(dataset_v0_3->acquisition.waves.begin(),
                                    dataset_v0_3->acquisition.waves.end(),
                                    [old_wave = old_event->transmit_setup.wave.lock()](
                                        const std::shared_ptr<urx::v0_3::Wave>& wave_i) {
                                      return old_wave.get() == wave_i.get();
                                    });
        const auto& old_wave = *it_wave;
        urx::Wave& new_wave = new_event.transmit_setup.wave;
        new_wave.type = old_to_new_wave_type.find(old_wave->wave_type) != old_to_new_wave_type.end()
                            ? old_to_new_wave_type.at(old_wave->wave_type)
                            : urx::Wave::WaveType::UNDEFINED;
        new_wave.time_zero = old_wave->time_zero;
        new_wave.time_zero_reference_point.x = old_wave->time_zero_reference_point.x;
        new_wave.time_zero_reference_point.y = old_wave->time_zero_reference_point.y;
        new_wave.time_zero_reference_point.z = old_wave->time_zero_reference_point.z;

        switch (new_wave.type) {
          case urx::Wave::WaveType::CONVERGING_WAVE:
          case urx::Wave::WaveType::DIVERGING_WAVE: {
            new_wave.parameters = {old_wave->origin.translation.x, old_wave->origin.translation.y,
                                   old_wave->origin.translation.z};
            break;
          }
          case urx::Wave::WaveType::PLANE_WAVE: {
            new_wave.parameters = {old_wave->origin.rotation.x, old_wave->origin.rotation.y,
                                   old_wave->origin.rotation.z};
            break;
          }
          case urx::Wave::WaveType::CYLINDRICAL_WAVE: {
            new_wave.parameters = {old_wave->origin.translation.x, old_wave->origin.translation.y,
                                   old_wave->origin.translation.z, old_wave->origin.rotation.x,
                                   old_wave->origin.rotation.y,    old_wave->origin.rotation.z};
            break;
          }
          default: {
            break;
          }
        }

        new_event.transmit_setup.delays = old_wave->channel_delays;

        new_event.transmit_setup.active_elements.reserve(old_wave->channel_mapping.size());
        std::transform(old_wave->channel_mapping.begin(), old_wave->channel_mapping.end(),
                       std::back_inserter(new_event.transmit_setup.active_elements),
                       [](const std::vector<uint32_t>& value) {
                         std::vector<uint32_t> retval_i;
                         retval_i.reserve(value.size());
                         std::transform(value.begin(), value.end(), std::back_inserter(retval_i),
                                        [](uint32_t value2) { return value2; });
                         return retval_i;
                       });

        new_event.transmit_setup.excitations.reserve(old_wave->channel_excitations.size());
        std::transform(old_wave->channel_excitations.begin(), old_wave->channel_excitations.end(),
                       std::back_inserter(new_event.transmit_setup.excitations),
                       [new_excitations = retval->acquisition.excitations,
                        old_excitations = dataset_v0_3->acquisition.excitation](
                           const std::weak_ptr<urx::v0_3::Excitation>& value) {
                         auto it_exc =
                             std::find_if(old_excitations.begin(), old_excitations.end(),
                                          [exc_ptr = value.lock().get()](
                                              const std::shared_ptr<urx::v0_3::Excitation>& exc_i) {
                                            return exc_ptr == exc_i.get();
                                          });
                         return new_excitations[std::distance(old_excitations.begin(), it_exc)];
                       });

        auto it_probe2 = std::find_if(dataset_v0_3->acquisition.probes.begin(),
                                      dataset_v0_3->acquisition.probes.end(),
                                      [old_probe = old_event->receive_setup.probe.lock()](
                                          const std::shared_ptr<urx::v0_3::Probe>& probe_i) {
                                        return old_probe.get() == probe_i.get();
                                      });
        new_event.receive_setup.probe =
            retval->acquisition
                .probes[std::distance(dataset_v0_3->acquisition.probes.begin(), it_probe2)];

        new_event.receive_setup.time_offset = old_event->receive_setup.time_offset;
        new_event.receive_setup.sampling_frequency = old_event->receive_setup.sampling_frequency;
        new_event.receive_setup.number_samples = old_event->receive_setup.nb_samples;

        new_event.receive_setup.active_elements.reserve(
            old_event->receive_setup.channel_mapping.size());
        std::transform(old_event->receive_setup.channel_mapping.begin(),
                       old_event->receive_setup.channel_mapping.end(),
                       std::back_inserter(new_event.receive_setup.active_elements),
                       [](const std::vector<uint32_t>& value) {
                         std::vector<uint32_t> retval_i;
                         retval_i.reserve(value.size());
                         std::transform(value.begin(), value.end(), std::back_inserter(retval_i),
                                        [](uint32_t value2) { return value2; });
                         return retval_i;
                       });

        new_event.receive_setup.tgc_sampling_frequency =
            old_event->receive_setup.tgc_sampling_frequency;
        new_event.receive_setup.tgc_profile = old_event->receive_setup.tgc_profile;
        new_event.receive_setup.modulation_frequency =
            old_event->receive_setup.modulation_frequency;

        new_group->sequence.push_back(std::move(new_event));
      }
    }

    retval->acquisition.groups.push_back(new_group);
  }

  retval->acquisition.groups_data.reserve(dataset_v0_3->acquisition.group_data.size());
  for (const auto& old_group_data : dataset_v0_3->acquisition.group_data) {
    GroupData new_group_data;

    auto group_probe = std::find_if(
        dataset_v0_3->acquisition.groups.begin(), dataset_v0_3->acquisition.groups.end(),
        [old_group =
             old_group_data->group.lock()](const std::shared_ptr<urx::v0_3::IGroup>& group_i) {
          return old_group.get() == group_i.get();
        });
    new_group_data.group =
        retval->acquisition
            .groups[std::distance(dataset_v0_3->acquisition.groups.begin(), group_probe)];

    new_group_data.raw_data = old_group_data->data;
    new_group_data.group_timestamp = static_cast<double>(old_group_data->timestamp);

    new_group_data.sequence_timestamps.reserve(old_group_data->sequence_timestamps.size());
    std::transform(old_group_data->sequence_timestamps.begin(),
                   old_group_data->sequence_timestamps.end(),
                   std::back_inserter(new_group_data.sequence_timestamps),
                   [](uint64_t value) { return static_cast<double>(value); });

    new_group_data.event_timestamps.reserve(old_group_data->event_timestamps.size());
    std::transform(old_group_data->event_timestamps.begin(), old_group_data->event_timestamps.end(),
                   std::back_inserter(new_group_data.event_timestamps),
                   [](const std::vector<uint64_t>& value) {
                     std::vector<double> retval_i;
                     retval_i.reserve(value.size());
                     std::transform(value.begin(), value.end(), std::back_inserter(retval_i),
                                    [](uint64_t value2) { return static_cast<double>(value2); });
                     return retval_i;
                   });

    negateSamplesReceivedOnRows<short>(new_group_data);

    retval->acquisition.groups_data.push_back(std::move(new_group_data));
  }

  return retval;
}
}  // namespace

std::shared_ptr<urx::Dataset> Upgrade::LoadFromFile(const std::string& filename) {
  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);

  auto readVersion = [](const H5::
#if H5_VERS_MAJOR == 1 && H5_VERS_MINOR <= 8
                            CommonFG
#else
                            H5Location
#endif
                                & location) {
    const H5::Group group_version(location.openGroup("version"));
    const H5::StrType datatype(H5::PredType::NATIVE_INT);
    const H5::DataSet dataset_major = group_version.openDataSet("major");
    int major;
    dataset_major.read(&major, datatype);
    const H5::DataSet dataset_minor = group_version.openDataSet("minor");
    int minor;
    dataset_minor.read(&minor, datatype);

    return std::tuple(major, minor);
  };

  // Check v0_2 format.
  if (file.nameExists("version")) {
    auto [major, minor] = readVersion(file);

    if (major == urx::v0_2::URX_VERSION_MAJOR && minor == urx::v0_2::URX_VERSION_MINOR) {
      const H5::Group group_channel_data(file.openGroup("channel_data"));
      const H5::DataSet dataset = group_channel_data.openDataSet("data");
      const H5::DataType datatype2 = dataset.getDataType();

      if (datatype2 == H5::PredType::NATIVE_FLOAT) {
        return ConvertV0_2<float>(filename);
      }
      if (datatype2 == H5::PredType::NATIVE_SHORT) {
        return ConvertV0_2<short>(filename);
      }
    }

    if (major == urx::v0_3::URX_VERSION_MAJOR && minor == urx::v0_3::URX_VERSION_MINOR) {
      return ConvertV0_3(filename);
    }

    if (major == urx::URX_VERSION_MAJOR && minor == urx::URX_VERSION_MINOR) {
      return urx::utils::io::Reader::loadFromFile(filename);
    }
  } else if (file.nameExists("dataset")) {
    const H5::Group group_version(file.openGroup("dataset"));
    auto [major, minor] = readVersion(group_version);

    if (major == urx::URX_VERSION_MAJOR && minor == urx::URX_VERSION_MINOR) {
      return urx::utils::io::Reader::loadFromFile(filename);
    }
  }

  return {};
}

}  // namespace urx::utils::io
