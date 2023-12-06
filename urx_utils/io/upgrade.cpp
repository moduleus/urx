#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
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
#include <urx/v0_2/dataset.h>
#include <urx/v0_2/linear_array.h>
#include <urx/v0_2/matrix_array.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/rca_array.h>
#include <urx/v0_2/reader.h>
#include <urx/v0_2/types.h>
#include <urx/v0_2/urx.h>
#include <urx/v0_2/wave.h>
#include <urx/vector.h>
#include <urx/wave.h>
#include <urx_utils/io/upgrade.h>

namespace urx {

namespace {
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

  for (const auto& probe : dataset_v0_2->channelData().probes()) {
    const std::shared_ptr<urx::Probe> new_probe = std::make_shared<urx::Probe>();
    new_probe->description = "";

    if (std::dynamic_pointer_cast<urx::v0_2::LinearArray>(probe)) {
      new_probe->type = urx::Probe::ProbeType::LINEAR;
    } else if (std::dynamic_pointer_cast<urx::v0_2::MatrixArray>(probe)) {
      new_probe->type = urx::Probe::ProbeType::MATRIX;
    } else if (std::dynamic_pointer_cast<urx::v0_2::RcaArray>(probe)) {
      new_probe->type = urx::Probe::ProbeType::RCA;
    } else {
      new_probe->type = urx::Probe::ProbeType::UNDEFINED;
    }

    new_probe->transform.rotation.x = probe->transform().rotation().x();
    new_probe->transform.rotation.y = probe->transform().rotation().y();
    new_probe->transform.rotation.z = probe->transform().rotation().z();
    new_probe->transform.translation.x = probe->transform().translation().x();
    new_probe->transform.translation.y = probe->transform().translation().y();
    new_probe->transform.translation.z = probe->transform().translation().z();

    for (const auto& impulse_response : probe->impulseResponses()) {
      const std::shared_ptr<urx::ImpulseResponse> new_impulse_response =
          std::make_shared<urx::ImpulseResponse>();

      auto opt_sampling = impulse_response.samplingFrequency();
      new_impulse_response->sampling_frequency =
          opt_sampling.has_value() ? *opt_sampling : urx::DoubleNan::NaN;
      std::transform(impulse_response.data().begin(), impulse_response.data().end(),
                     std::back_inserter(new_impulse_response->data),
                     [](urx::v0_2::MetadataType value) -> double { return value; });
      new_impulse_response->units = impulse_response.units();
      new_impulse_response->time_offset = impulse_response.initialTime();

      new_probe->impulse_responses.push_back(new_impulse_response);
    }

    new_probe->elements.reserve(probe->elements().size());
    size_t element_i = 0;
    for (const auto& element : probe->elements()) {
      const auto opt_x = element.x();
      const auto opt_y = element.y();
      const auto opt_z = element.z();
      const double x = opt_x.has_value() ? *opt_x : urx::DoubleNan::NaN;
      const double y = opt_y.has_value() ? *opt_y : urx::DoubleNan::NaN;
      const double z = opt_z.has_value() ? *opt_z : urx::DoubleNan::NaN;

      const std::shared_ptr<urx::ElementGeometry> new_element_geometry =
          std::make_shared<urx::ElementGeometry>();
      if (auto probe_linear = std::dynamic_pointer_cast<urx::v0_2::LinearArray>(probe);
          probe_linear) {
        const auto opt_height = element.x();
        const double height = opt_height.has_value() ? *opt_height : urx::DoubleNan::NaN;
        new_element_geometry->perimeter.emplace_back(x - probe_linear->pitch() / 2, y - height / 2,
                                                     z);
        new_element_geometry->perimeter.emplace_back(x - probe_linear->pitch() / 2, y + height / 2,
                                                     z);
        new_element_geometry->perimeter.emplace_back(x + probe_linear->pitch() / 2, y + height / 2,
                                                     z);
        new_element_geometry->perimeter.emplace_back(x + probe_linear->pitch() / 2, y - height / 2,
                                                     z);
      } else if (auto probe_rca = std::dynamic_pointer_cast<urx::v0_2::RcaArray>(probe);
                 probe_rca) {
        if (element_i < probe_rca->numberElementsX()) {
          new_element_geometry->perimeter.emplace_back(
              x - probe_rca->pitchX() / 2,
              y - probe_rca->pitchY() / 2 * probe_rca->numberElementsY(), z);
          new_element_geometry->perimeter.emplace_back(
              x - probe_rca->pitchX() / 2,
              y + probe_rca->pitchY() / 2 * probe_rca->numberElementsY(), z);
          new_element_geometry->perimeter.emplace_back(
              x + probe_rca->pitchX() / 2,
              y + probe_rca->pitchY() / 2 * probe_rca->numberElementsY(), z);
          new_element_geometry->perimeter.emplace_back(
              x + probe_rca->pitchX() / 2,
              y - probe_rca->pitchY() / 2 * probe_rca->numberElementsY(), z);
        } else {
          new_element_geometry->perimeter.emplace_back(
              x - probe_rca->pitchX() / 2 * probe_rca->numberElementsX(),
              y - probe_rca->pitchY() / 2, z);
          new_element_geometry->perimeter.emplace_back(
              x - probe_rca->pitchX() / 2 * probe_rca->numberElementsX(),
              y + probe_rca->pitchY() / 2, z);
          new_element_geometry->perimeter.emplace_back(
              x + probe_rca->pitchX() / 2 * probe_rca->numberElementsX(),
              y + probe_rca->pitchY() / 2, z);
          new_element_geometry->perimeter.emplace_back(
              x + probe_rca->pitchX() / 2 * probe_rca->numberElementsX(),
              y - probe_rca->pitchY() / 2, z);
        }
      } else if (auto probe_matrix = std::dynamic_pointer_cast<urx::v0_2::MatrixArray>(probe);
                 probe_matrix) {
        new_element_geometry->perimeter.emplace_back(x - probe_matrix->pitchX() / 2,
                                                     y - probe_matrix->pitchY() / 2, z);
        new_element_geometry->perimeter.emplace_back(x - probe_matrix->pitchX() / 2,
                                                     y + probe_matrix->pitchY() / 2, z);
        new_element_geometry->perimeter.emplace_back(x + probe_matrix->pitchX() / 2,
                                                     y + probe_matrix->pitchY() / 2, z);
        new_element_geometry->perimeter.emplace_back(x + probe_matrix->pitchX() / 2,
                                                     y - probe_matrix->pitchY() / 2, z);
      }

      new_probe->element_geometries.push_back(new_element_geometry);

      urx::Element element_new;
      element_new.transform.translation = {x, y, z};
      element_new.element_geometry = new_probe->element_geometries.back();
      element_new.impulse_response = element_i < new_probe->impulse_responses.size()
                                         ? new_probe->impulse_responses[element_i]
                                         : nullptr;
      new_probe->elements.push_back(std::move(element_new));

      element_i++;
    }

    retval->acquisition.probes.push_back(new_probe);
  }

  std::vector<size_t> map_wave_to_excitation;
  size_t excitation_i = 0;
  for (const auto& wave : dataset_v0_2->channelData().uniqueWaves()) {
    const auto& excitation = wave->excitation();

    const std::shared_ptr<urx::Excitation> new_excitation = std::make_shared<urx::Excitation>();
    const auto& pulse_shape = excitation.pulseShape();
    new_excitation->pulse_shape = pulse_shape.has_value() ? *pulse_shape : "";
    const auto& sampling_frequency = excitation.samplingFrequency();
    new_excitation->sampling_frequency =
        sampling_frequency.has_value() ? *sampling_frequency : urx::DoubleNan::NaN;
    const auto& transmit_frequency = excitation.transmitFrequency();
    new_excitation->transmit_frequency =
        transmit_frequency.has_value() ? *transmit_frequency : urx::DoubleNan::NaN;
    std::transform(excitation.waveform().begin(), excitation.waveform().end(),
                   std::back_inserter(new_excitation->waveform),
                   [](urx::v0_2::MetadataType value) -> double { return value; });
    auto it = std::find(retval->acquisition.excitations.begin(),
                        retval->acquisition.excitations.end(), new_excitation);
    if (it == std::end(retval->acquisition.excitations)) {
      retval->acquisition.excitations.push_back(new_excitation);
      map_wave_to_excitation.push_back(excitation_i);
      excitation_i++;
    } else {
      map_wave_to_excitation.push_back(std::distance(retval->acquisition.excitations.begin(), it));
    }
  }

  size_t wave_i = 0;
  for (const auto& wave : dataset_v0_2->channelData().uniqueWaves()) {
    const std::shared_ptr<urx::Wave> new_wave = std::make_shared<urx::Wave>();

    new_wave->type = old_to_new_wave_type.contains(wave->waveType())
                         ? old_to_new_wave_type.at(wave->waveType())
                         : urx::Wave::WaveType::UNDEFINED;

    new_wave->parameters = {wave->origin().rotation().x(),    wave->origin().rotation().y(),
                            wave->origin().rotation().z(),    wave->origin().translation().x(),
                            wave->origin().translation().y(), wave->origin().translation().z()};

    new_wave->channel_mapping.reserve(dataset_v0_2->channelData().uniqueEvents().size());
    new_wave->channel_excitations = std::vector<std::weak_ptr<Excitation>>(
        dataset_v0_2->channelData().uniqueEvents().size(),
        retval->acquisition.excitations[map_wave_to_excitation[wave_i]]);
    for (const auto& event : dataset_v0_2->channelData().uniqueEvents()) {
      std::vector<uint32_t> new_channel_mapping;
      std::transform(event->transmitSetup().channelMapping().begin(),
                     event->transmitSetup().channelMapping().end(),
                     std::back_inserter(new_channel_mapping),
                     [](int value) -> uint32_t { return value; });
      new_wave->channel_mapping.push_back(new_channel_mapping);

      if (event->transmitSetup().getTransmitWave().wave().lock().get() == wave.get()) {
        new_wave->time_zero = event->transmitSetup().getTransmitWave().timeOffset();
        new_wave->time_zero_reference_point = {0, 0, 0};
      }
    }

    new_wave->channel_delays = std::vector<double>(new_wave->channel_excitations.size());

    retval->acquisition.waves.push_back(new_wave);

    wave_i++;
  }

  {
    const std::shared_ptr<Group> group = std::make_shared<Group>();
    group->sampling_type = urx::Group::SamplingType::RF;
    if constexpr (std::is_same_v<T, short>) {
      group->data_type = urx::Group::DataType::INT16;
    } else {
      group->data_type = urx::Group::DataType::FLOAT;
    }
    group->description = "";
    for (const auto& sequence : dataset_v0_2->channelData().sequence()) {
      const auto& event = sequence.evenement().lock();

      Event new_event;
      new_event.transmit_setup.time_offset = 0;
      new_event.transmit_setup.probe_transform = {};
      auto it_probe = std::find_if(dataset_v0_2->channelData().probes().begin(),
                                   dataset_v0_2->channelData().probes().end(),
                                   [probe = event->transmitSetup().probe().lock()](
                                       const std::shared_ptr<urx::v0_2::Probe>& probe_i) {
                                     return probe.get() == probe_i.get();
                                   });
      new_event.transmit_setup.probe =
          retval->acquisition
              .probes[std::distance(dataset_v0_2->channelData().probes().begin(), it_probe)];

      auto it_wave = std::find_if(dataset_v0_2->channelData().uniqueWaves().begin(),
                                  dataset_v0_2->channelData().uniqueWaves().end(),
                                  [wave = event->transmitSetup().getTransmitWave().wave().lock()](
                                      const std::shared_ptr<urx::v0_2::Wave>& wave_i2) {
                                    return wave.get() == wave_i2.get();
                                  });
      new_event.transmit_setup.wave =
          retval->acquisition
              .waves[std::distance(dataset_v0_2->channelData().uniqueWaves().begin(), it_wave)];

      it_probe = std::find_if(dataset_v0_2->channelData().probes().begin(),
                              dataset_v0_2->channelData().probes().end(),
                              [probe = event->receiveSetup().probe().lock()](
                                  const std::shared_ptr<urx::v0_2::Probe>& probe_i) {
                                return probe.get() == probe_i.get();
                              });
      new_event.receive_setup.probe =
          retval->acquisition
              .probes[std::distance(dataset_v0_2->channelData().probes().begin(), it_probe)];
      new_event.receive_setup.time_offset = event->receiveSetup().timeOffset();
      new_event.receive_setup.sampling_frequency = event->receiveSetup().samplingFrequency();
      std::transform(event->receiveSetup().tgcProfile().begin(),
                     event->receiveSetup().tgcProfile().end(),
                     std::back_inserter(new_event.receive_setup.tgc_profile),
                     [](urx::v0_2::MetadataType value) -> double { return value; });
      const auto& tgc_sampling_frequency = event->receiveSetup().tgcSamplingFrequency();
      new_event.receive_setup.tgc_sampling_frequency =
          tgc_sampling_frequency.has_value() ? *tgc_sampling_frequency : urx::DoubleNan::NaN;
      const auto& modulation_frequency = event->receiveSetup().modulationFrequency();
      new_event.receive_setup.modulation_frequency =
          modulation_frequency.has_value() ? *modulation_frequency : urx::DoubleNan::NaN;
      new_event.receive_setup.probe_transform = {};

      std::vector<uint32_t> new_channel_mapping;
      std::transform(event->receiveSetup().channelMapping().begin(),
                     event->receiveSetup().channelMapping().end(),
                     std::back_inserter(new_channel_mapping),
                     [](int value) -> uint32_t { return value; });
      new_event.receive_setup.channel_mapping.push_back(new_channel_mapping);

      new_event.receive_setup.number_samples = dataset_v0_2->channelData().numberOfSamples();

      group->sequence.push_back(std::move(new_event));
    }

    retval->acquisition.groups.push_back(group);
  }

  {
    const std::shared_ptr<GroupData> group_data = std::make_shared<GroupData>();

    group_data->group = retval->acquisition.groups[0];

    const size_t sequences = dataset_v0_2->channelData().numberOfFrames();

    group_data->raw_data =
        std::make_shared<urx::RawDataVector<T>>(std::move(dataset_v0_2->channelData().data()));

    group_data->sequence_timestamps.reserve(sequences);
    const auto opt_rate = dataset_v0_2->channelData().repetitionRate();
    const double sequence_rate = opt_rate.has_value() ? *opt_rate : urx::DoubleNan::NaN;
    for (const size_t i : std::views::iota(0ULL, sequences)) {
      group_data->sequence_timestamps.push_back(i / sequence_rate);

      std::vector<double> event_offset;
      for (const auto& event : retval->acquisition.groups[0]->sequence) {
        event_offset.push_back(i / sequence_rate + event.transmit_setup.time_offset);
      }
      group_data->event_timestamps.push_back(std::move(event_offset));
    }

    group_data->group_timestamp = 0;

    retval->acquisition.groups_data.push_back(group_data);
  }

  return retval;
}
}  // namespace

std::shared_ptr<urx::Dataset> Upgrade::LoadFromFile(const std::string& filename) {
  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);

  // Check v0_2 format.
  if (file.nameExists("version")) {
    const H5::Group group_version(file.openGroup("version"));
    const H5::StrType datatype(H5::PredType::NATIVE_INT);
    const H5::DataSet dataset_major = group_version.openDataSet("major");
    int major;
    dataset_major.read(&major, datatype);
    const H5::DataSet dataset_minor = group_version.openDataSet("minor");
    int minor;
    dataset_minor.read(&minor, datatype);

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
  }

  return {};
}

}  // namespace urx
