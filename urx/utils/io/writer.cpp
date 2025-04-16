#include <algorithm>
#include <vector>

#include <urx/dataset.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_impl.h>

namespace urx::utils::io::writer {

void saveToFile(const std::string& filename, const Dataset& dataset) {
  urx::utils::io::Writer<Dataset, AllTypeInVariant> writer(filename, &dataset, getMemberMap());
  writer.write();
}

}  // namespace urx::utils::io::writer
