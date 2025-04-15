#include <memory>
#include <vector>

#include <urx/dataset.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  urx::utils::io::Reader<Dataset, AllTypeInVariant> reader(filename, getMemberMap());
  reader.read();
  return reader.getDataset();
}

}  // namespace urx::utils::io::reader
