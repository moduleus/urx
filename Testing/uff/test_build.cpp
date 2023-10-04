#include <uff/acquisition.h>
#include <uff/aperture.h>
#include <uff/dataset.h>
#include <uff/element.h>
#include <uff/element_geometry.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/impulse_response.h>
#include <uff/linear_array.h>
#include <uff/matrix_array.h>
#include <uff/probe.h>
#include <uff/rca_array.h>
#include <uff/receive_setup.h>
#include <uff/event.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/version.h>

#include <catch2/catch.hpp>
namespace uff::test {

TEST_CASE("Instantiate all classes to check build", "[test_build]") {
  const Acquisition acquisition;
  const Aperture aperture;
  const Dataset dataset;
  const ElementGeometry element_geometry;
  const Element element;
  const Excitation excitation;
  const GroupData group_data;
  const Group group;
  const ImpulseResponse impulse_response;
  const LinearArray linear_array;
  const MatrixArray matrix_array;
  const Probe probe;
  const RcaArray rca_array;
  const ReceiveSetup receive_setup;
  const Event event;
  const Transform transform;
  const TransmitSetup transmit_setup;
  const WaveType waveType = WaveType::PLANE_WAVE;
  const Vector2D<double> vector2D;
  const Vector3D<double> vector3D;
  const Version version;
}

}  // namespace uff::test
