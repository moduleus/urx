#include <H5Cpp.h>

#include <urx/v0_5/probe.h>
#include <urx/v0_5/reader.h>
#include <urx_utils/io/upgrade.h>

namespace urx {

namespace {
template <typename T>
std::shared_ptr<urx::Dataset> ConvertV0_5(const std::string& filename) {
  urx::v0_5::Reader<T> reader;
  reader.setFileName(filename);
  reader.updateMetadata();
  std::shared_ptr<urx::v0_5::Dataset<T>> dataset_v0_5 = reader.dataset();

  std::shared_ptr<urx::Dataset> retval = std::make_shared<urx::Dataset>();

  retval->acquisition.authors = dataset_v0_5->channelData().authors();
  retval->acquisition.description = dataset_v0_5->channelData().description();
  retval->acquisition.local_time = dataset_v0_5->channelData().localTime();
  retval->acquisition.country_code = dataset_v0_5->channelData().countryCode();
  retval->acquisition.system = dataset_v0_5->channelData().system();
  retval->acquisition.sound_speed = dataset_v0_5->channelData().soundSpeed();
  // retval->acquisition.timestamp = 0.;

  for (const auto& probe : dataset_v0_5->channelData().probes()) {
    std::shared_ptr<urx::Probe> new_probe = std::make_shared<urx::Probe>();
    new_probe->description = "";

    // TODO : add algorithm to detect probe type.
    new_probe->type = urx::Probe::ProbeType::UNDEFINED;

    new_probe->transform.rotation.x = probe->transform().rotation().x();
    new_probe->transform.rotation.y = probe->transform().rotation().y();
    new_probe->transform.rotation.z = probe->transform().rotation().z();
    new_probe->transform.translation.x = probe->transform().translation().x();
    new_probe->transform.translation.y = probe->transform().translation().y();
    new_probe->transform.translation.z = probe->transform().translation().z();

    for (const auto& element_geometry : probe->elementGeometries()) {
      std::shared_ptr<urx::ElementGeometry> new_element_geometry =
          std::make_shared<urx::ElementGeometry>();
      // TODO FIELD
      new_probe->element_geometries.push_back(new_probe);
    }
    for (const auto& impulse_response : probe->impulseResponses()) {
      std::shared_ptr<urx::ImpulseResponse> new_impulse_response =
          std::make_shared<urx::ImpulseResponse>();

      auto opt_sampling = impulse_response.samplingFrequency();
      new_impulse_response->sampling_frequency =
          opt_sampling.has_value() ? *opt_sampling : urx::DoubleNan::NaN;
      std::transform(impulse_response.data().begin(), impulse_response.data().end(),
                     std::back_inserter(new_impulse_response->data),
                     [](urx::v0_5::MetadataType value) -> double { return value; });
      new_impulse_response->units = impulse_response.units();
      new_impulse_response->time_offset = impulse_response.initialTime();

      new_probe->impulse_responses.push_back(new_impulse_response);
    }
    /*
  /// List of all the elements in the probe
  std::vector<Element> elements;
*/
    retval->acquisition.probes.push_back(new_probe);
  }
  for (const auto& probe : dataset_v0_5->channelData().uniqueWaves()) {
    // TODO waves
  }

  /*

  /// List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> excitations;

  /// List of all group present in the acquisition
  std::vector<std::shared_ptr<Group>> groups;

  /// List of all data acquired by the running groups in the acquisition
  std::vector<std::shared_ptr<GroupData>> groups_data;
  
  */
  return retval;
}
}  // namespace

std::shared_ptr<urx::Dataset> Upgrade::LoadFromFile(const std::string& filename) {
  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);

  // Check v0_5 format.
  if (file.nameExists("version")) {
    const H5::Group group_version(file.openGroup("version"));
    const H5::StrType datatype(H5::PredType::NATIVE_INT);
    const H5::DataSet dataset_major = group_version.openDataSet("major");
    int major;
    dataset_major.read(&major, datatype);
    const H5::DataSet dataset_minor = group_version.openDataSet("minor");
    int minor;
    dataset_minor.read(&minor, datatype);

    if (major == urx::v0_5::URX_VERSION_MAJOR && minor == urx::v0_5::URX_VERSION_MINOR) {
      const H5::Group group_channel_data(file.openGroup("channel_data"));
      const H5::DataSet dataset = group_channel_data.openDataSet("data");
      const H5::DataType datatype = dataset.getDataType();

      if (datatype == H5::PredType::NATIVE_FLOAT) {
        return ConvertV0_5<float>(filename);
      } else if (datatype == H5::PredType::NATIVE_SHORT) {
        return ConvertV0_5<short>(filename);
      }
    }
  }

  return {};
}

}  // namespace urx
