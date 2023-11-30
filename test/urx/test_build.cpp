#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
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

namespace urx::test {

TEST_CASE("Instantiate all classes to check build", "[test_build]") {
  const Acquisition acquisition;
  const Dataset dataset;
  const ElementGeometry element_geometry;
  const Element element;
  const Excitation excitation;
  const GroupData group_data;
  const Group group;
  const ImpulseResponse impulse_response;
  const Probe probe;
  const ReceiveSetup receive_setup;
  const Event event;
  const Transform transform;
  const TransmitSetup transmit_setup;
  const Wave wave;
  const Vector2D<double> vector2D;
  const Vector3D<double> vector3D;
  const Version version;
}

}  // namespace urx::test
