#pragma once

#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <uff/detail/compare.h>
#include <uff/detail/double_nan.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>

namespace uff {

/**
 * @brief Main UFF class containing all macro information
 * 
 * The acquisition class is used to store the memory allocation of all main UFF classes.
 * All classes used in the acquisition must be stored in its lists. 
 * If not, the acquisition will be corrupted and retrieved data cannot be processed.
 * 
 * It also reference all data retrieved from the experimentation.
 * 
 * Context information should be described in it, in order to keep track of experience. 
 */
struct Acquisition {
  bool operator==(const Acquisition& other) const = default;

  /// Experimentation authors
  std::string authors;

  /// Description of the experimentation
  std::string description;

  /// Date of the experimentation acquisition following ISO 8601
  std::string local_time;

  /// Contry identification following ISO 3166-1
  std::string country_code;

  /// Description of the system used to acquired the dataset
  std::string system;

  /// Reference sound speed for Tx and Rx events [m/s]
  /// You must provide the sound speed inside the studied material in order to process the data
  DoubleNan sound_speed;

  /// Timestamp of the launch of the acquisition [s]
  DoubleNan timestamp;

  /// List of all the probes used to transmit/receive sequences in the acquisition
  std::vector<std::shared_ptr<Probe>> probes;

  /// List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> excitations;

  /// List of all the unique waves used in the sequences in the acquisition
  std::vector<std::shared_ptr<Wave>> waves;

  /// List of all group present in the acquisition
  std::vector<std::shared_ptr<Group>> groups;

  /// List of all data acquired by the running groups in the acquisition
  std::vector<std::shared_ptr<GroupData>> groups_data;
};

}  // namespace uff
