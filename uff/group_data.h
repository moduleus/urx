#pragma once

#include <uff/group.h>

#include <complex>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include <uff/detail/compare.h>

namespace uff {

/**
 * @brief The UFF class containing all the retrieved data from the acquisition
 */
struct GroupData {
  template <class... Args>
  struct VecDataType {
    using real_and_complex = std::variant<std::vector<Args>..., std::vector<std::complex<Args>>...>;
  };

  using VecDataTypeVariant = VecDataType<int16_t, int32_t, float, double>::real_and_complex;

  bool operator==(const GroupData& other) const = default;

  /// Reference of the group whose data have been retrieved
  std::weak_ptr<Group> group = std::weak_ptr<Group>();

  /// Data are organized as raw_data[Group repetition count][Number of event][Number of channel activated during the event][Number of samples]
  /// Data are in 1D array since the dimensions of the data array is dynamic for the number of activated channels and for the number of samples
  /// Create your helper to deduce the dimensions or use Uff_Utils to access correctly to the data
  VecDataTypeVariant raw_data;

  /// Timestamp of the group launch [s]
  double group_timestamp = UFF_NAN;

  /// Timestamp list for each sequence launch [s]
  std::vector<double> sequence_timestamps;

  /// Timestamp 2D list for each event of each sequence launch [s]
  std::vector<std::vector<double>> event_timestamps;
};

}  // namespace uff
