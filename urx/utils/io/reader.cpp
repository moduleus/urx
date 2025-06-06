#include <memory>
#include <vector>  // IWYU pragma: keep

#include <urx/dataset.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  std::shared_ptr<Dataset> dataset = std::make_shared<Dataset>();
  urx::utils::io::ReaderDataset reader;
  reader.read(filename, *dataset);
  return dataset;
}

}  // namespace urx::utils::io::reader
