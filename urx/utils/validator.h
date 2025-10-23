#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ios>
#include <iosfwd>
#include <istream>
#include <iterator>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/utils/common.h>
#include <urx/utils/time_helper.h>
#include <urx/vector.h>
#include <urx/wave.h>

#define UNUSED(x) (void)(x)

namespace urx::utils {

template <typename Derived, typename Dataset>
class ValidatorBase {
 public:
  virtual ~ValidatorBase() = default;

  virtual void check(Dataset& dataset) {
    check(dataset.acquisition);

    for (size_t i = 0; i < dataset.acquisition.probes.size(); ++i) {
      if (dataset.acquisition.probes.at(i)) {
        check(*dataset.acquisition.probes.at(i), "Probe " + std::to_string(i) + ": ");
      } else {
        static_cast<Derived*>(this)->nullPointer(
            dataset.acquisition.probes.at(i),
            "Probe " + std::to_string(i) + ": Probe pointer is null.");
      }
    }

    for (size_t i = 0; i < dataset.acquisition.excitations.size(); ++i) {
      if (dataset.acquisition.excitations.at(i)) {
        check(*(dataset.acquisition.excitations.at(i)), "Excitation " + std::to_string(i) + ": ");
      } else {
        static_cast<Derived*>(this)->nullPointer(
            dataset.acquisition.excitations.at(i),
            "Excitation " + std::to_string(i) + ": Excitation pointer is null.");
      }
    }

    for (size_t i = 0; i < dataset.acquisition.groups.size(); ++i) {
      if (dataset.acquisition.groups.at(i)) {
        check(*dataset.acquisition.groups.at(i), "Group " + std::to_string(i) + ": ");

        for (size_t j = 0; j < dataset.acquisition.groups.at(i)->sequence.size(); ++j) {
          if (!common::getElementIndex(
                   dataset.acquisition.probes,
                   dataset.acquisition.groups.at(i)->sequence.at(j).receive_setup.probe, false)
                   .has_value()) {
            static_cast<Derived*>(this)->wrongPointer(
                dataset.acquisition.groups.at(i)->sequence.at(j).receive_setup.probe,
                "Group " + std::to_string(i) + ": Event " + std::to_string(j) +
                    ": Receive setup: Wrong probe pointer.\n"
                    "dataset.acquisition.group[" +
                    std::to_string(i) + "].sequence[" + std::to_string(j) +
                    "].receive_setup.probe");
          }

          if (!common::getElementIndex(
                   dataset.acquisition.probes,
                   dataset.acquisition.groups.at(i)->sequence.at(j).transmit_setup.probe, false)
                   .has_value()) {
            static_cast<Derived*>(this)->wrongPointer(
                dataset.acquisition.groups.at(i)->sequence.at(j).transmit_setup.probe,

                "Group " + std::to_string(i) + ": Event " + std::to_string(j) +
                    ": Transmit setup: Wrong probe pointer.\n"
                    "dataset.acquisition.group[" +
                    std::to_string(i) + "].sequence[" + std::to_string(j) +
                    "].transmit_setup.probe");
          }

          for (size_t k = 0;
               k <
               dataset.acquisition.groups.at(i)->sequence.at(j).transmit_setup.excitations.size();
               ++k) {
            if (!common::getElementIndex(
                     dataset.acquisition.excitations,
                     dataset.acquisition.groups.at(i)->sequence.at(j).transmit_setup.excitations.at(
                         k),
                     false)
                     .has_value()) {
              static_cast<Derived*>(this)->wrongPointer(
                  dataset.acquisition.groups.at(i)->sequence.at(j).transmit_setup.excitations.at(k),
                  "Group " + std::to_string(i) + ": Event " + std::to_string(j) + ": Excitation " +
                      std::to_string(k) +
                      ": Wrong excitation pointer.\n"
                      "dataset.acquisition.group[" +
                      std::to_string(i) + "].sequence[" + std::to_string(j) +
                      "].transmit_setup.excitations[" + std::to_string(k) + "]");
            }
          }
        }
      } else {
        static_cast<Derived*>(this)->nullPointer(
            dataset.acquisition.groups.at(i),
            "Group " + std::to_string(i) + ": Group pointer is null.");
      }
    }

    if constexpr (std::is_same_v<Dataset, urx::Dataset>) {
      for (size_t i = 0; i < dataset.acquisition.groups_data.size(); ++i) {
        check(dataset.acquisition.groups_data.at(i));

        if (!common::getElementIndex(dataset.acquisition.groups,
                                     dataset.acquisition.groups_data.at(i).group, false)
                 .has_value()) {
          static_cast<Derived*>(this)->wrongPointer(dataset.acquisition.groups_data.at(i).group,
                                                    "GroupData " + std::to_string(i) +
                                                        ": Wrong group pointer.\n"
                                                        "dataset.acquisition.group_data[" +
                                                        std::to_string(i) + "].group");
        }
      }
    }
  }

  virtual void check(decltype(Dataset::acquisition)& acq) {
    if (acq.excitations.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          acq.excitations,
          "Empty excitation vector: Acquisition excitations should "
          "contain at least one element.");
    }
    if (acq.groups.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          acq.groups,
          "Empty group vector: Acquisition groups should contain at least one element.");
    }
    if constexpr (std::is_same_v<Dataset, urx::Dataset>) {
      if (acq.groups_data.empty()) {
        static_cast<Derived*>(this)->vectorEmpty(
            acq.groups_data,
            "Empty group data vector: Acquisition groups data should "
            "contain at least one element.");
      }
    }
    if (time_helper::isIso8601(acq.local_time)) {
      static_cast<Derived*>(this)->notIso8601(acq.local_time,
                                              "Local time is not compliant with Iso 8601");
    }
    if (time_helper::isIso3166(acq.country_code)) {
      static_cast<Derived*>(this)->notIso3166(acq.country_code,
                                              "Country code is not compliant with Iso 3166");
    }
    if (std::isnan(acq.timestamp)) {
      static_cast<Derived*>(this)->valueNotSet(acq.timestamp, "Acquisition timestamp is not set");
    }
    if (acq.timestamp < 0) {
      static_cast<Derived*>(this)->valueNotSet(
          acq.timestamp, "Acquisition timestamp is not set correctly. Value must be positive.");
    }
  }

  virtual void check(Probe& probe, const std::string& identifier_message_prefix = "") {
    if (probe.impulse_responses.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          probe.impulse_responses, identifier_message_prefix +
                                       "Empty impulse responses vector: Probe impulse "
                                       "responses should contain at least one element.");
    } else {
      for (size_t i = 0; i < probe.impulse_responses.size(); ++i) {
        if (probe.impulse_responses.at(i)) {
          check(*probe.impulse_responses.at(i),
                identifier_message_prefix + "Impulse Response " + std::to_string(i) + ": ");
        } else {
          static_cast<Derived*>(this)->nullPointer(probe.impulse_responses.at(i),
                                                   identifier_message_prefix + "Impulse response " +
                                                       std::to_string(i) +
                                                       ": Impulse response pointer is null.");
        }
      }
    }

    if (probe.element_geometries.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          probe.element_geometries,
          identifier_message_prefix +
              "Empty element geometries vector: Probe element geometries of "
              "the probe should contain at least one element.");
    } else {
      for (size_t i = 0; i < probe.element_geometries.size(); ++i) {
        if (probe.element_geometries.at(i)) {
          check(*probe.element_geometries.at(i),
                identifier_message_prefix + "Element geometry " + std::to_string(i) + ": ");
        } else {
          static_cast<Derived*>(this)->nullPointer(probe.element_geometries.at(i),
                                                   identifier_message_prefix + "Element geometry " +
                                                       std::to_string(i) +
                                                       ": Element geometry pointer is null.");
        }
      }
    }

    if (probe.elements.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          probe.elements, identifier_message_prefix +
                              "Empty elements vector: Probe elements of "
                              "the probe should contain at least one element.");
    } else {
      for (size_t i = 0; i < probe.elements.size(); ++i) {
        if (!common::getElementIndex(probe.element_geometries,
                                     probe.elements.at(i).element_geometry, false)
                 .has_value()) {
          static_cast<Derived*>(this)->wrongPointer(probe.elements.at(i).element_geometry,
                                                    identifier_message_prefix + "Element " +
                                                        std::to_string(i) +
                                                        ": Wrong element geometry pointer.");
        }

        if (!common::getElementIndex(probe.impulse_responses, probe.elements.at(i).impulse_response,
                                     false)
                 .has_value()) {
          static_cast<Derived*>(this)->wrongPointer(probe.elements.at(i).impulse_response,
                                                    identifier_message_prefix + "Element " +
                                                        std::to_string(i) +
                                                        ": Wrong impulse response pointer.");
        }
      }
    }
  }

  using ExcitationT = typename decltype(Dataset::acquisition.excitations)::value_type::element_type;
  virtual void check(ExcitationT& excitation, const std::string& identifier_message_prefix = "") {
    if (std::isnan(excitation.sampling_frequency)) {
      static_cast<Derived*>(this)->valueNotSet(
          excitation.sampling_frequency,
          identifier_message_prefix + "Sampling frequency: " + "Value is not set.");
    }
    if (excitation.sampling_frequency < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          excitation.sampling_frequency, identifier_message_prefix + "Sampling frequency: " +
                                             "Value is not set correctly. Value must be positive.");
    }

    if (std::isnan(excitation.transmit_frequency)) {
      static_cast<Derived*>(this)->valueNotSet(
          excitation.transmit_frequency,
          identifier_message_prefix + "Transmit frequency: " + "Value is not set.");
    }
    if (excitation.transmit_frequency < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          excitation.transmit_frequency, identifier_message_prefix + "Transmit frequency: " +
                                             "Value is not set correctly. Value must be positive.");
    }

    if (excitation.waveform.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          excitation.waveform, identifier_message_prefix +
                                   "Empty waveform: Waveform should contain at least one point.");
    }
  }

  virtual void check(ElementGeometry& eg, const std::string& identifier_message_prefix = "") {
    if (eg.perimeter.size() < 3) {
      static_cast<Derived*>(this)->vectorWrongSize(
          eg.perimeter, identifier_message_prefix +
                            "Perimeter : perimeter should contain at least three points.");
    }
  }

  virtual void check(ImpulseResponse& ir, const std::string& identifier_message_prefix = "") {
    if (std::isnan(ir.sampling_frequency)) {
      static_cast<Derived*>(this)->valueNotSet(
          ir.sampling_frequency,
          identifier_message_prefix + "Sampling frequency: " + "Value is not set.");
    }
    if (ir.sampling_frequency < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          ir.sampling_frequency, identifier_message_prefix + "Sampling frequency: " +
                                     "Value is not set correctly. Value must be positive.");
    }

    if (std::isnan(ir.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(
          ir.time_offset, identifier_message_prefix + "Time offset: " + "Value is not set.");
    }

    if (ir.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          ir.time_offset, identifier_message_prefix + "Time offset: " +
                              "Value is not set correctly. Value must be positive or zero.");
    }

    if (ir.data.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          ir.data,
          identifier_message_prefix + "Empty data: Data should contain at least one value.");
    }
  }

  using GroupT = typename decltype(Dataset::acquisition.groups)::value_type::element_type;
  virtual void check(GroupT& group, const std::string& identifier_message_prefix = "") {
    if (group.sequence.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          group.sequence, identifier_message_prefix +
                              "Empty sequence: sequence should contain at least one element.");
    } else {
      for (size_t i = 0; i < group.sequence.size(); ++i) {
        check(group.sequence.at(i), group.sampling_type,
              identifier_message_prefix + "Event " + std::to_string(i) + ": ");
      }
    }

    if (std::isnan(group.sound_speed)) {
      static_cast<Derived*>(this)->valueNotSet(
          group.sound_speed, identifier_message_prefix + "Sound speed: " + "Value is not set.");
    }

    if (group.sound_speed < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          group.sound_speed, identifier_message_prefix + "Sound speed: " +
                                 "Value is not set correctly. Value must be positive or zero.");
    }

    if (group.data_type == DataType::UNDEFINED) {
      static_cast<Derived*>(this)->valueNotSet(
          group.sound_speed, identifier_message_prefix + "Data type: " +
                                 "Value is not set correctly. Value must not be UNDEFINED.");
    }

    if (group.sampling_type == SamplingType::UNDEFINED) {
      static_cast<Derived*>(this)->valueNotSet(
          group.sound_speed, identifier_message_prefix + "Sampling type: " +
                                 "Value is not set correctly. Value must not be UNDEFINED.");
    }
  }

  using EventT = typename decltype(GroupT::sequence)::value_type;
  virtual void check(EventT& event, const SamplingType& type,
                     const std::string& identifier_message_prefix = "") {
    check(event.receive_setup, type, identifier_message_prefix + "Receive Setup : ");
    check(event.transmit_setup, identifier_message_prefix + "Transmit Setup : ");
  }

  virtual void check(GroupData& group_data, const std::string& identifier_message_prefix = "") {
    if (std::isnan(group_data.group_timestamp)) {
      static_cast<Derived*>(this)->valueNotSet(
          group_data.group_timestamp,
          identifier_message_prefix + "Group timestamp: " + "Value is not set.");
    }

    if (group_data.sequence_timestamps.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          group_data.sequence_timestamps,
          identifier_message_prefix +
              "Empty sequence timestamp vector: Group data sequence timestamps should "
              "contain at least one element.");
    } else {
      for (size_t i = 0; i < group_data.sequence_timestamps.size(); ++i) {
        if (std::isnan(group_data.sequence_timestamps.at(i))) {
          static_cast<Derived*>(this)->valueNotSet(group_data.sequence_timestamps.at(i),
                                                   identifier_message_prefix +
                                                       "Sequence timestamp " + std::to_string(i) +
                                                       ": " + "Value is not set.");
        }
      }
    }

    if (group_data.event_timestamps.empty()) {
      static_cast<Derived*>(this)->vectorEmpty(
          group_data.event_timestamps,
          identifier_message_prefix +
              "Empty event timestamp vector: Group data event timestamps should "
              "contain at least one element.");
    } else {
      for (size_t i = 0; i < group_data.event_timestamps.size(); ++i) {
        if (group_data.event_timestamps.empty()) {
          static_cast<Derived*>(this)->vectorEmpty(
              group_data.event_timestamps[i], identifier_message_prefix +
                                                  "Empty event timestamp [" + std::to_string(i) +
                                                  "]:  Group data event timestamps should "
                                                  "contain at least one element.");
        } else {
          for (size_t j = 0; j < group_data.event_timestamps.at(i).size(); ++j) {
            if (std::isnan(group_data.event_timestamps.at(i).at(j))) {
              static_cast<Derived*>(this)->valueNotSet(
                  group_data.event_timestamps[i][j],
                  identifier_message_prefix + "Event timestamp [" + std::to_string(i) + "][" +
                      std::to_string(j) + "]: " + "Value is not set.");
            }
          }
        }
      }
    }
  }

  using ReceiveSetupT = decltype(EventT::receive_setup);
  virtual void check(ReceiveSetupT& rs, const SamplingType& type,
                     const std::string& identifier_message_prefix = "") {
    if (std::isnan(rs.modulation_frequency) && type == SamplingType::IQ) {
      static_cast<Derived*>(this)->valueNotSet(
          rs.modulation_frequency,
          identifier_message_prefix + "Modulation frequency: " + "Value is not set.");
    }
    if (rs.modulation_frequency < 0 && type == SamplingType::IQ) {
      static_cast<Derived*>(this)->valueNotPositive(
          rs.modulation_frequency, identifier_message_prefix + "Modulation frequency: " +
                                       "Value is not set correctly. Value must be positive.");
    }

    if (std::isnan(rs.sampling_frequency)) {
      static_cast<Derived*>(this)->valueNotSet(
          rs.sampling_frequency,
          identifier_message_prefix + "Sampling frequency: " + "Value is not set.");
    }
    if (rs.sampling_frequency < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          rs.sampling_frequency, identifier_message_prefix + "Sampling frequency: " +
                                     "Value is not set correctly. Value must be positive.");
    }

    if (std::isnan(rs.tgc_sampling_frequency) && !rs.tgc_profile.empty()) {
      static_cast<Derived*>(this)->valueNotSet(
          rs.tgc_sampling_frequency, identifier_message_prefix + "TGC sampling frequency: " +
                                         "Value is not set while TGC profile is defined.");
    }
    if (rs.tgc_sampling_frequency < 0 && !rs.tgc_profile.empty()) {
      static_cast<Derived*>(this)->valueNotPositive(
          rs.tgc_sampling_frequency, identifier_message_prefix + "TGC sampling frequency: " +
                                         "Value is not set correctly while TGC profile is "
                                         "defined. Value must be positive.");
    }

    if (std::isnan(rs.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(
          rs.time_offset, identifier_message_prefix + "Time offset: " + "Value is not set.");
    }

    if (rs.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          rs.time_offset, identifier_message_prefix + "Time offset: " +
                              "Value is not set correctly. Value must be positive or zero.");
    }

    for (size_t i = 0; i < rs.active_elements.size(); ++i) {
      for (size_t j = 0; j < rs.active_elements.at(i).size(); ++j) {
        if (!rs.probe.expired() &&
            rs.active_elements.at(i).at(j) < rs.probe.lock()->elements.size()) {
          static_cast<Derived*>(this)->valueOutOfRange(
              rs.active_elements.at(i).at(j),
              identifier_message_prefix + "Active element[" + std::to_string(i) + "][[" +
                  std::to_string(j) + "]: " + "Value is out of range. Referenced probe only has " +
                  std::to_string(j) + " channels.");
        }
      }
    }
  }

  using TransmitSetupT = decltype(EventT::transmit_setup);
  virtual void check(TransmitSetupT& ts, const std::string& identifier_message_prefix = "") {
    if (std::isnan(ts.time_offset)) {
      static_cast<Derived*>(this)->valueNotSet(
          ts.time_offset, identifier_message_prefix + "Time offset: " + "Value is not set.");
    }

    if (ts.time_offset < 0) {
      static_cast<Derived*>(this)->valueNotPositive(
          ts.time_offset, identifier_message_prefix + "Time offset: " +
                              "Value is not set correctly. Value must be positive or zero.");
    }

    check(ts.wave, identifier_message_prefix + "Wave :");

    if (ts.delays.size() != ts.active_elements.size() ||
        ts.delays.size() != ts.excitations.size()) {
      static_cast<Derived*>(this)->vectorWrongSize(
          ts.delays, identifier_message_prefix +
                         "Delays: Delays should contain the same number of "
                         "elements as active elements and excitation.");
    }

    if (ts.active_elements.size() != ts.delays.size() ||
        ts.active_elements.size() != ts.excitations.size()) {
      static_cast<Derived*>(this)->vectorWrongSize(
          ts.active_elements, identifier_message_prefix +
                                  "Active elements: Active elements should contain the same number "
                                  "of elements as delays and excitation.");
    }

    if (ts.excitations.size() != ts.active_elements.size() ||
        ts.excitations.size() != ts.delays.size()) {
      static_cast<Derived*>(this)->vectorWrongSize(
          ts.excitations, identifier_message_prefix +
                              "Excitations: Excitations should contain the same number of "
                              "elements as active elements and delays.");
    }

    for (size_t i = 0; i < ts.active_elements.size(); ++i) {
      for (size_t j = 0; j < ts.active_elements.at(i).size(); ++j) {
        if (!ts.probe.expired() &&
            ts.active_elements.at(i).at(j) < ts.probe.lock()->elements.size()) {
          static_cast<Derived*>(this)->valueOutOfRange(
              ts.active_elements.at(i).at(j),
              identifier_message_prefix + "Active element[" + std::to_string(i) + "][[" +
                  std::to_string(j) + "]: " + "Value is out of range. Referenced probe only has " +
                  std::to_string(j) + " channels.");
        }
      }
    }
  }

  virtual void check(Wave& w, const std::string& identifier_message_prefix = "") {
    if (std::isnan(w.time_zero)) {
      static_cast<Derived*>(this)->valueNotSet(
          w.time_zero, identifier_message_prefix + "Time zero: " + "Value is not set.");
    }

    if (w.type == WaveType::CONVERGING_WAVE && w.parameters.size() != 3) {
      static_cast<Derived*>(this)->vectorWrongSize(
          w.parameters, identifier_message_prefix + "Parameters: " +
                            "Parameters should contains 3 values. 3D Cartesian "
                            "coordinates of wave's focal point.");
    }

    if (w.type == WaveType::CYLINDRICAL_WAVE && w.parameters.size() != 6) {
      static_cast<Derived*>(this)->vectorWrongSize(
          w.parameters, identifier_message_prefix + "Parameters: " +
                            "Parameters should contains 6 values. The "
                            "cylindrical wave is parametrized by an axis "
                            "(n_x,n_y,n_z) and a point (x,y,z), all "
                            "expressed in 3D cartesian coordinates.");
    }

    if (w.type == WaveType::DIVERGING_WAVE && w.parameters.size() != 3) {
      static_cast<Derived*>(this)->vectorWrongSize(
          w.parameters, identifier_message_prefix + "Parameters: " +
                            "Parameters should contains 3 values. 3D Cartesian coordinates of "
                            "the wave's virtual source.");
    }

    if (w.type == WaveType::PLANE_WAVE && w.parameters.size() != 3) {
      static_cast<Derived*>(this)->vectorWrongSize(
          w.parameters, identifier_message_prefix + "Parameters: " +
                            "Parameters should contains 3 values. 3D Cartesian coordinates of "
                            "the direction vector of the plane. The vector must be unit-length: "
                            "sqrt(n_x^2 + n_y^2 + n_z^2) = 1");
    }
  }
};

/// Return a list of error messages detected in the checked class
/// If the output vector is empty then all is ok
template <template <typename, typename...> class ValidatorBaseT, typename Dataset>
class ValidatorReportBase
    : public ValidatorBaseT<ValidatorReportBase<ValidatorBaseT, Dataset>, Dataset> {
 public:
  ~ValidatorReportBase() override = default;

  void check(Dataset& dataset) override {
    _output.clear();
    ValidatorBaseT<ValidatorReportBase<ValidatorBaseT, Dataset>, Dataset>::check(dataset);
  }

  void throwIfFailure() const {
    if (_output.empty()) {
      return;
    }

    throw std::runtime_error(getOutputAsString());
  }

  const std::vector<std::string>& getOutput() const { return _output; }
  std::string getOutputAsString() const {
    std::stringstream ss;
    ss << "Target dataset contains incoherent data:\n";
    for (const std::string& s : _output) {
      ss << s << "\n";
    }
    return ss.str();
  }

  template <typename Type>
  void valueNotSet(const Type& value, const std::string& message) {
    UNUSED(value);
    _output.push_back(message);
  }

  template <typename Type>
  void valueNotPositive(const Type& value, const std::string& message) {
    UNUSED(value);
    _output.push_back(message);
  }

  template <typename Type>
  void valueOutOfRange(const Type& value, const std::string& message) {
    UNUSED(value);
    _output.push_back(message);
  }

  template <typename Type>
  void nullPointer(const std::shared_ptr<Type>& ptr, const std::string& message) {
    UNUSED(ptr);
    _output.push_back(message);
  }

  template <typename Type>
  void wrongPointer(const std::weak_ptr<Type>& ptr, const std::string& message) {
    UNUSED(ptr);
    _output.push_back(message);
  }

  template <typename Type>
  void vectorEmpty(const std::vector<Type>& vec, const std::string& message) {
    UNUSED(vec);
    _output.push_back(message);
  }

  template <typename Type>
  void vectorWrongSize(const std::vector<Type>& vec, const std::string& message) {
    UNUSED(vec);
    _output.push_back(message);
  }

  void notIso8601(const std::string& local_time, const std::string& message) {
    UNUSED(local_time);
    _output.push_back(message);
  }
  void notIso3166(const std::string& country_code, const std::string& message) {
    UNUSED(country_code);
    _output.push_back(message);
  }

 private:
  std::vector<std::string> _output;
};

using ValidatorReport = ValidatorReportBase<ValidatorBase, Dataset>;

}  // namespace urx::utils
