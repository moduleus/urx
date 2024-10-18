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
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/version.h>

namespace urx::utils::io::reader {

std::shared_ptr<Dataset> loadFromFile(const std::string& filename) {
  auto dataset = std::make_shared<Dataset>();

  const H5::H5File file(filename.data(), H5F_ACC_RDONLY);
  std::vector<std::function<void()>> async_weak_assign;
  MapToSharedPtr map_to_shared_ptr{{nameTypeid<Group>(), &dataset->acquisition.groups},
                                   {nameTypeid<Probe>(), &dataset->acquisition.probes},
                                   {nameTypeid<Excitation>(), &dataset->acquisition.excitations},
                                   {nameTypeid<GroupData>(), &dataset->acquisition.groups_data},
                                   {nameTypeid<std::function<void()>>(), &async_weak_assign}};

  DeserializeHdf5<Dataset, AllTypeInVariant>::f("dataset", *dataset, file, map_to_shared_ptr,
                                                getMemberMap());

  for (auto& funct : async_weak_assign) {
    funct();
  }

  if (dataset->version.major != urx::URX_VERSION_MAJOR) {
    return {};
  }

  dataset->version.minor = urx::URX_VERSION_MINOR;
  dataset->version.patch = urx::URX_VERSION_PATCH;

  return dataset;
}

}  // namespace urx::utils::io::reader
