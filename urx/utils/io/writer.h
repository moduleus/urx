#pragma once

#include <string>

#include <urx/dataset.h>
#include <urx/utils/export.h>
#include <urx/utils/io/writer_options.h>

namespace urx::utils::io::writer {

URX_UTILS_EXPORT void saveToFile(const std::string& filename, const Dataset& dataset,
                                 const WriterOptions& options = {});

}  // namespace urx::utils::io::writer
