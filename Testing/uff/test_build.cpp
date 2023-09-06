#include <uff/acquisition.h>
#include <uff/aperture.h>
#include <uff/channel_data.h>
#include <uff/dataset.h>
#include <uff/destination_link.h>
#include <uff/element.h>
#include <uff/element_geometry.h>
#include <uff/event_data.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/igroup.h>
#include <uff/impulse_response.h>
#include <uff/linear_array.h>
#include <uff/matrix_array.h>
#include <uff/probe.h>
#include <uff/rca_array.h>
#include <uff/receive_setup.h>
#include <uff/sequence.h>
#include <uff/sequence_data.h>
#include <uff/super_group.h>
#include <uff/timed_event.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_in.h>
#include <uff/trigger_location.h>
#include <uff/trigger_out.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/version.h>

#include <catch2/catch.hpp>
namespace uff::test {

TEST_CASE("Instantiate all classes to check build", "[test_build]") {
  const Acquisition acquisition;
  const Aperture aperture;
  const ChannelData<int16_t> channel_Data;
  const Dataset dataset;
  const DestinationLink destination_link;
  const ElementGeometry element_geometry;
  const Element element;
  const EventData<int16_t> event_data;
  const Excitation excitation;
  const GroupData<int16_t> group_data;
  const Group group;
  const IGroup igroup;
  const ImpulseResponse impulse_response;
  const LinearArray linear_array;
  const MatrixArray matrix_array;
  const Probe probe;
  const RcaArray rca_array;
  const ReceiveSetup receive_setup;
  const SequenceData<int16_t> sequence_data;
  const Sequence sequence;
  const SuperGroup super_group;
  const TimedEvent timed_event;
  const Transform transform;
  const TransmitSetup transmit_setup;
  const TriggerIn trigger_in;
  const TriggerLocation trigger_location;
  const TriggerOut trigger_out;
  const WaveType waveType = WaveType::PLANE_WAVE;
  const Vector2D<double> vector2D;
  const Vector3D<double> vector3D;
  const Version version;
}

}  // namespace uff::test
