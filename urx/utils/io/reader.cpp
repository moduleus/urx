#include <memory>
#include <vector>  // IWYU pragma: keep

#include <urx/dataset.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  urx::utils::io::ReaderDataset<Dataset, AllTypeInVariant, urx::utils::io::ReaderBase> reader(
      filename);
  reader.read();
  return reader.getDataset();
}

}  // namespace urx::utils::io::reader
