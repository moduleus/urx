#pragma once

#include <cstddef>
#include <memory>

#include <urx/config.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/utils/export.h>
#include <urx/utils/group_helper.h>

#ifdef URX_WITH_HDF5
#include <H5Cpp.h>

#include <urx/utils/io/enums.h>
#endif

namespace urx::utils {

template <template <typename> class Container>
URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type,
                                                              SamplingType sampling_type,
                                                              size_t size);

template <template <typename> class Container>
URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type,
                                                              SamplingType sampling_type,
                                                              size_t size, void* buffer);

class RawDataHelper {
 public:
  RawDataHelper(const urx::RawData* raw_data) : _raw_data(raw_data) {}

  size_t sizeofSample() const {
    return urx::utils::group_helper::sizeofSample(_raw_data->getSamplingType(),
                                                  _raw_data->getDataType());
  }

#ifdef URX_WITH_HDF5
  const H5::PredType& getHdf5Type() const {
    return urx::utils::io::enums::dataTypeToH5PredType(_raw_data->getDataType());
  }
#endif

 private:
  const urx::RawData* _raw_data;
};

}  // namespace urx::utils
