#include <algorithm>
#include <iterator>
#include <vector>

#include <urx/dataset.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_impl.h>
#include <urx/utils/io/writer_options.h>
#include <urx/utils/validator.h>

namespace urx::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset, const WriterOptions& options) {
  urx::utils::io::WriterDataset writer;
  writer.setOptions(options);
  if (options.getCheckData()) {
    ValidatorReport validator;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    validator.check(const_cast<Dataset&>(dataset));
    validator.throwIfFailure();
  }
  writer.write(filename, dataset);
}

}  // namespace urx::utils::io::writer
