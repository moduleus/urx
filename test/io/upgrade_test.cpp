#include <complex>
#include <cstdlib>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>
#include <urx_utils/io/upgrade.h>

namespace urx::test {

TEST_CASE("Upgrade HDF5 file", "[hdf5_writer]") {
  const std::string dir = (TEST_RESOURCE_DIR);

  urx::Upgrade::LoadFromFile(dir + "v_0_5_short.urx");
}

}  // namespace urx::test
