#include <functional>
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
#include <urx/urx.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/version.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  urx::utils::io::Reader<Dataset, AllTypeInVariant> reader(filename, getMemberMap());
  return reader.getDataset();
}

}  // namespace urx::utils::io::reader
