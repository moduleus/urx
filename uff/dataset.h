#pragma once

#include <uff/acquisition.h>
#include <uff/version.h>

namespace uff {

/**
 * @brief The UFF Dataset class
 */
class Dataset {
 public:
  // CTOR & DTOR
  Dataset() = default;
  Dataset(const Dataset&) = default;
  Dataset(Dataset&&) = default;
  ~Dataset() = default;

  // Operators
  Dataset& operator=(const Dataset& other) noexcept = default;
  Dataset& operator=(Dataset&& other) noexcept = default;
  inline bool operator==(const Dataset& other) const {
    return ((_version == other._version) && (_acquisition == other._acquisition));
  }
  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

  // Accessors
  inline Acquisition& acquisition() { return _acquisition; }
  inline const Acquisition& acquisition() const { return _acquisition; }
  inline void setAcquisition(const Acquisition& acquisition) { _acquisition = acquisition; }
  inline void setAcquisition(Acquisition&& acquisition) { _acquisition = std::move(acquisition); }

  inline const Version& version() const { return _version; }
  inline void setVersion(const Version& version) { _version = version; }

  // Members
 private:
  Version _version;
  Acquisition _acquisition;
};

}  // namespace uff
