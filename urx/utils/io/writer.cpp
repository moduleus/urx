#include <urx/dataset.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_impl.h>
#include <urx/utils/io/writer_options.h>

namespace urx::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset) {
  urx::utils::io::WriterDataset writer;
  writer.write(filename, dataset);
}

void saveToFile(const std::string& filename, const Dataset& dataset, const WriterOptions& options) {
  urx::utils::io::WriterDataset writer;
  writer.setOptions(options);
  writer.write(filename, dataset);
}

}  // namespace urx::utils::io::writer
