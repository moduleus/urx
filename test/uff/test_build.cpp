#include <uff/acquisition.h>
#include <uff/dataset.h>
#include <uff/element.h>
#include <uff/element_geometry.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/impulse_response.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/uff.h>
#include <uff/vector.h>
#include <uff/version.h>
#include <uff/wave.h>

#include <catch2/catch_test_macros.hpp>

namespace uff::test {

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

}  // namespace uff::test
