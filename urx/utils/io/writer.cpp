#include <algorithm>
#include <memory>
#include <typeindex>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_impl.h>

namespace urx::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset) {
  urx::utils::io::Writer<Dataset, AllTypeInVariant> writer(filename, dataset);
}

}  // namespace urx::utils::io::writer
