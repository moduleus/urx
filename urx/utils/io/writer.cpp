#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_impl.h>

namespace urx::utils::io::writer {
void saveToFile(const std::string& filename, const Dataset& dataset) {
  const H5::H5File file(filename.data(), H5F_ACC_TRUNC);
  MapToSharedPtr map_to_shared_ptr{{nameTypeid<Group>(), &dataset.acquisition.groups},
                                   {nameTypeid<Probe>(), &dataset.acquisition.probes},
                                   {nameTypeid<Excitation>(), &dataset.acquisition.excitations},
                                   {nameTypeid<GroupData>(), &dataset.acquisition.groups_data}};

  SerializeHdf5<Dataset, AllTypeInVariant>::f("dataset", dataset, file, map_to_shared_ptr,
                                              member_name);
}

}  // namespace urx::utils::io::writer
