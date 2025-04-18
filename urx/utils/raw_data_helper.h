#pragma once

#include <cstddef>
#include <memory>

#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/utils/export.h>

namespace urx::utils {

URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type,
                                                              SamplingType sampling_type,
                                                              size_t size);

}
