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
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  auto dataset = std::make_shared<Dataset>();

  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);
  MapToSharedPtr map_to_shared_ptr{{nameTypeid<Group>(), &dataset->acquisition.groups},
                                   {nameTypeid<Probe>(), &dataset->acquisition.probes},
                                   {nameTypeid<Excitation>(), &dataset->acquisition.excitations},
                                   {nameTypeid<GroupData>(), &dataset->acquisition.groups_data}};

  DeserializeHdf5<Dataset, AllTypeInVariant>::f("dataset", *dataset, file, map_to_shared_ptr,
                                                member_name);

  return dataset;
}

}  // namespace urx::utils::io::reader
