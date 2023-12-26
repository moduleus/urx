clear all
feature('DefaultCharacterSet','UTF-8');

dataset = urx.Dataset();

dataset.version.minor = 123;
assert(dataset.version.minor == 123); % uin32_t
dataset.version.major = 456;
dataset.version.patch = 789;

version = urx.Version();
version.minor = 111;
version.major = 852;
version.patch = 963;
dataset.version = version;
assert(dataset.version.minor == 111);
assert(dataset.version.major == 852);
assert(dataset.version.patch == 963);
version.minor = 1;
version.major = 42;
version.patch = 73;
assert(dataset.version.minor == 1);
assert(dataset.version.major == 42);
assert(dataset.version.patch == 73);
dataset.version.minor = 7;
dataset.version.major = 48;
dataset.version.patch = 159;
assert(version.minor == 7);
assert(version.major == 48);
assert(version.patch == 159);


dataset.acquisition.authors = 'AuthOr';
dataset.acquisition.description = '康Description🐜';
assert(strcmp(dataset.acquisition.description, '康Description🐜')); % std::string
dataset.acquisition.localTime = '2019-06-15T15:53:00';
dataset.acquisition.countryCode = 'FR';
dataset.acquisition.system = '???';
dataset.acquisition.soundSpeed = 1500.;
assert(dataset.acquisition.soundSpeed == 1500.); %double

probe = urx.Probe();
probe.description = 'Probe 1';
probe.type = urx.Probe.ProbeType.MATRIX;
assert(probe.type == urx.Probe.ProbeType.MATRIX); %enum
probe.transform.rotation.x = 1;
probe.transform.rotation.y = 2;
probe.transform.rotation.z = 3;
probe.transform.translation.x = 1.2;
probe.transform.translation.y = 2.2;
probe.transform.translation.z = 3.2;

elementGeometry = urx.ElementGeometry();
vector = urx.Vector3D();
vector.x = 0.15;
vector.y = 0.23;
vector.z = 543;
elementGeometry.perimeter.pushBack(vector);
vector.x = 2.2;
vector.y = 1.4;
vector.z = 0.2;
elementGeometry.perimeter.pushBack(vector);
probe.elementGeometries.pushBack(elementGeometry);

elementGeometry = urx.ElementGeometry();
vector = urx.Vector3D();
vector.x = 2.2;
vector.y = 1.4;
vector.z = 0.2;
elementGeometry.perimeter.pushBack(vector);
vector.x = 1.2;
vector.y = 2.4;
vector.z = 543;
elementGeometry.perimeter.pushBack(vector);
probe.elementGeometries.pushBack(elementGeometry);

impulseResponse = urx.ImpulseResponse();
impulseResponse.samplingFrequency = 20000001;
impulseResponse.data.pushBack(1.2);
impulseResponse.data.pushBack(1.3);
impulseResponse.data.pushBack(1.4);
impulseResponse.units = 'meter';
impulseResponse.timeOffset = 10000;
probe.impulseResponses.pushBack(impulseResponse);

impulseResponse = urx.ImpulseResponse();
impulseResponse.samplingFrequency = NaN;
impulseResponse.data.pushBack(2.2);
impulseResponse.data.pushBack(2.3);
impulseResponse.data.pushBack(2.4);
impulseResponse.units = 'milli';
impulseResponse.timeOffset = NaN;
probe.impulseResponses.pushBack(impulseResponse);

element = urx.Element();
element.transform.rotation.x = 11;
element.transform.rotation.y = 22;
element.transform.rotation.z = 33;
element.transform.translation.x = 44155;
element.transform.translation.y = 44255;
element.transform.translation.z = 44355;
element.elementGeometry = probe.elementGeometries.data(1);
element.impulseResponse = probe.impulseResponses.data(2);
probe.elements.pushBack(element);

element = urx.Element();
element.transform.rotation.x = 33;
element.transform.rotation.y = 22;
element.transform.rotation.z = 44;
element.transform.translation.x = 77;
element.transform.translation.y = 88;
element.transform.translation.z = 987;
element.elementGeometry = probe.elementGeometries.data(2);
element.impulseResponse = probe.impulseResponses.data(1);
probe.elements.pushBack(element);

dataset.acquisition.probes.pushBack(probe);


probe = urx.Probe();
probe.description = 'Probe 2';
probe.type = 123;
assert(probe.type == 123);
probe.transform.rotation.x = 3;
probe.transform.rotation.y = 4;
probe.transform.rotation.z = 5;
probe.transform.translation.x = 3.2;
probe.transform.translation.y = 4.2;
probe.transform.translation.z = 5.2;

elementGeometry = urx.ElementGeometry();
vector = urx.Vector3D();
vector.x = 123;
vector.y = 4.5;
vector.z = 5.4;
elementGeometry.perimeter.pushBack(vector);
vector.x = 1.1;
vector.y = 22.1;
vector.z = 5.4;
elementGeometry.perimeter.pushBack(vector);
probe.elementGeometries.pushBack(elementGeometry);

elementGeometry = urx.ElementGeometry();
vector = urx.Vector3D();
vector.x = 5.5;
vector.y = 6.6;
vector.z = 7.7;
elementGeometry.perimeter.pushBack(vector);
vector.x = 1.1;
vector.y = 0.0;
vector.z = 5.5;
elementGeometry.perimeter.pushBack(vector);
probe.elementGeometries.pushBack(elementGeometry);

impulseResponse = urx.ImpulseResponse();
impulseResponse.samplingFrequency = 20000011;
impulseResponse.data.pushBack(22.2);
impulseResponse.data.pushBack(3.3);
impulseResponse.data.pushBack(144.4);
impulseResponse.units = 'met55er';
impulseResponse.timeOffset = 100500;
probe.impulseResponses.pushBack(impulseResponse);

impulseResponse = urx.ImpulseResponse();
impulseResponse.samplingFrequency = 5000004402;
impulseResponse.data.pushBack(23.2);
impulseResponse.data.pushBack(2.53);
impulseResponse.data.pushBack(72.4);
impulseResponse.units = 'mi3lli';
impulseResponse.timeOffset = 200100;
probe.impulseResponses.pushBack(impulseResponse);

element = urx.Element();
element.transform.rotation.x = 99;
element.transform.rotation.y = 88;
element.transform.rotation.z = 77;
element.transform.translation.x = 741;
element.transform.translation.y = 852;
element.transform.translation.z = 963;
element.elementGeometry = probe.elementGeometries.data(1);
element.impulseResponse = probe.impulseResponses.data(2);
probe.elements.pushBack(element);

element = urx.Element();
element.transform.rotation.x = 33;
element.transform.rotation.y = 11;
element.transform.rotation.z = 22;
element.transform.translation.x = 86;
element.transform.translation.y = 753;
element.transform.translation.z = 42;
element.elementGeometry = probe.elementGeometries.data(2);
element.impulseResponse = probe.impulseResponses.data(1);
probe.elements.pushBack(element);

dataset.acquisition.probes.pushBack(probe);


excitation = urx.Excitation();
excitation.pulseShape = 'sinusoidal';
excitation.transmitFrequency = 123456789;
excitation.samplingFrequency = 987654321;
excitation.waveform.pushBack(0);
excitation.waveform.pushBack(0.1);
excitation.waveform.pushBack(0.2);
dataset.acquisition.excitations.pushBack(excitation);


excitation = urx.Excitation();
excitation.pulseShape = 'square wave';
excitation.transmitFrequency = 741085296;
excitation.samplingFrequency = 369258147;
excitation.waveform.pushBack(1);
excitation.waveform.pushBack(1.1);
excitation.waveform.pushBack(1.2);
dataset.acquisition.excitations.pushBack(excitation);


wave = urx.Wave();
wave.type = urx.Wave.WaveType.CYLINDRICAL_WAVE;
wave.timeZeroReferencePoint.x = 3;
wave.timeZeroReferencePoint.y = 4;
wave.timeZeroReferencePoint.z = 5;
wave.timeZero = 5.11;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
assert(channelMappingI.data(1), uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(3));
channelMappingI.pushBack(uint32(45));
assert(channelMappingI.data(4), uint32(45));
channelMappingI.pushBack(uint32(9));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(5));
wave.channelMapping.pushBack(channelMappingI);
assert(wave.channelMapping.data(1).data(4), uint32(45));

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(58));
channelMappingI.pushBack(uint32(961));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(8));
channelMappingI.pushBack(uint32(5));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

wave.channelExcitations.pushBack(dataset.acquisition.excitations.data(1));
wave.channelExcitations.pushBack(dataset.acquisition.excitations.data(2));
wave.channelDelays.pushBack(1.2);
wave.channelDelays.pushBack(45);
wave.channelDelays.pushBack(.26);
wave.channelDelays.pushBack(1.2);
wave.channelDelays.pushBack(5);
wave.parameters.pushBack(1);
wave.parameters.pushBack(.2);
wave.parameters.pushBack(4);
wave.parameters.pushBack(3);
wave.parameters.pushBack(6);
wave.parameters.pushBack(4.5);
dataset.acquisition.waves.pushBack(wave);








wave = urx.Wave();
wave.type = urx.Wave.WaveType.CONVERGING_WAVE;
wave.timeZeroReferencePoint.x = 5;
wave.timeZeroReferencePoint.y = 4;
wave.timeZeroReferencePoint.z = 4;
wave.timeZero = 5.11;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(45));
channelMappingI.pushBack(uint32(4));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(54));
channelMappingI.pushBack(uint32(53));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(66));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(25));
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(3));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(245));
channelMappingI.pushBack(uint32(6));
channelMappingI.pushBack(uint32(42));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(12));
channelMappingI.pushBack(uint32(12));
wave.channelMapping.pushBack(channelMappingI);

wave.channelExcitations.pushBack(dataset.acquisition.excitations.data(2));
wave.channelExcitations.pushBack(dataset.acquisition.excitations.data(1));
wave.channelDelays.pushBack(1.2);
wave.channelDelays.pushBack(.3);
wave.channelDelays.pushBack(.5);
wave.channelDelays.pushBack(10.4);
wave.parameters.pushBack(7);
wave.parameters.pushBack(53);
wave.parameters.pushBack(.2);
wave.parameters.pushBack(1);
wave.parameters.pushBack(.3);
wave.parameters.pushBack(5.6);
wave.parameters.pushBack(7);
dataset.acquisition.waves.pushBack(wave);


group = urx.Group();
group.samplingType = urx.Group.SamplingType.IQ;
group.dataType = urx.Group.DataType.INT16;

event = urx.Event();
event.transmitSetup.probe = dataset.acquisition.probes.data(1);
event.transmitSetup.probeTransform.rotation.x = 5;
event.transmitSetup.probeTransform.rotation.y = 8;
event.transmitSetup.probeTransform.rotation.z = 7;
event.transmitSetup.probeTransform.translation.x = 5.2;
event.transmitSetup.probeTransform.translation.y = 4.3;
event.transmitSetup.probeTransform.translation.z = 8.2;
event.transmitSetup.timeOffset = 120.2;
event.transmitSetup.wave = dataset.acquisition.waves.data(2);
event.receiveSetup.probe = dataset.acquisition.probes.data(2);
event.receiveSetup.probeTransform.rotation.x = 51;
event.receiveSetup.probeTransform.rotation.y = 85;
event.receiveSetup.probeTransform.rotation.z = 73;
event.receiveSetup.probeTransform.translation.x = 5.72;
event.receiveSetup.probeTransform.translation.y = 4.83;
event.receiveSetup.probeTransform.translation.z = 8.52;
event.receiveSetup.samplingFrequency = 125468;
event.receiveSetup.numberSamples = 4452;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(753));
channelMappingI.pushBack(uint32(8));
channelMappingI.pushBack(uint32(6));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(1));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(4));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(6));
channelMappingI.pushBack(uint32(21));
channelMappingI.pushBack(uint32(8));
channelMappingI.pushBack(uint32(9));
channelMappingI.pushBack(uint32(3));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(3));
channelMappingI.pushBack(uint32(6));
channelMappingI.pushBack(uint32(5));
event.receiveSetup.channelMapping.pushBack(channelMappingI);

event.receiveSetup.tgcProfile.pushBack(1.2);
event.receiveSetup.tgcProfile.pushBack(45.);
event.receiveSetup.tgcProfile.pushBack(4.5);
event.receiveSetup.tgcProfile.pushBack(-4.2);
event.receiveSetup.tgcProfile.pushBack(1);
event.receiveSetup.tgcProfile.pushBack(.5);
event.receiveSetup.tgcProfile.pushBack(4.6);
event.receiveSetup.tgcProfile.pushBack(1);
event.receiveSetup.tgcProfile.pushBack(2);
event.receiveSetup.tgcSamplingFrequency = 12.5;
event.receiveSetup.modulationFrequency = 125;
event.receiveSetup.timeOffset = 1205;
group.sequence.pushBack(event);

event = urx.Event();
event.transmitSetup.probe = dataset.acquisition.probes.data(2);
event.transmitSetup.probeTransform.rotation.x = 55;
event.transmitSetup.probeTransform.rotation.y = 48;
event.transmitSetup.probeTransform.rotation.z = 77;
event.transmitSetup.probeTransform.translation.x = 552;
event.transmitSetup.probeTransform.translation.y = 463;
event.transmitSetup.probeTransform.translation.z = 872;
event.transmitSetup.timeOffset = 12052;
event.transmitSetup.wave = dataset.acquisition.waves.data(1);
event.receiveSetup.probe = dataset.acquisition.probes.data(1);
event.receiveSetup.probeTransform.rotation.x = 5;
event.receiveSetup.probeTransform.rotation.y = 8;
event.receiveSetup.probeTransform.rotation.z = 3;
event.receiveSetup.probeTransform.translation.x = 52;
event.receiveSetup.probeTransform.translation.y = 43;
event.receiveSetup.probeTransform.translation.z = 52;
event.receiveSetup.samplingFrequency = 1268;
event.receiveSetup.numberSamples = 42;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(3));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(4));
channelMappingI.pushBack(uint32(5));
channelMappingI.pushBack(uint32(6));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(8));
channelMappingI.pushBack(uint32(9));
event.receiveSetup.channelMapping.pushBack(channelMappingI);

event.receiveSetup.tgcProfile.pushBack(1);
event.receiveSetup.tgcProfile.pushBack(.2);
event.receiveSetup.tgcProfile.pushBack(4.);
event.receiveSetup.tgcProfile.pushBack(5);
event.receiveSetup.tgcProfile.pushBack(.1);
event.receiveSetup.tgcProfile.pushBack(5);
event.receiveSetup.tgcProfile.pushBack(.45);
event.receiveSetup.tgcProfile.pushBack(5);
event.receiveSetup.tgcSamplingFrequency = 4.5;
event.receiveSetup.modulationFrequency = 4.58;
event.receiveSetup.timeOffset = 7.8;
group.sequence.pushBack(event);

group.description = 'group description';
dataset.acquisition.groups.pushBack(group);


group = urx.Group();
group.samplingType = urx.Group.SamplingType.RF;
group.dataType = urx.Group.DataType.DOUBLE;

event = urx.Event();
event.transmitSetup.probe = dataset.acquisition.probes.data(2);
event.transmitSetup.probeTransform.rotation.x = 55;
event.transmitSetup.probeTransform.rotation.y = 88;
event.transmitSetup.probeTransform.rotation.z = 67;
event.transmitSetup.probeTransform.translation.x = 5.72;
event.transmitSetup.probeTransform.translation.y = 4.83;
event.transmitSetup.probeTransform.translation.z = 8.52;
event.transmitSetup.timeOffset = 1202.2;
event.transmitSetup.wave = dataset.acquisition.waves.data(1);
event.receiveSetup.probe = dataset.acquisition.probes.data(1);
event.receiveSetup.probeTransform.rotation.x = 5101;
event.receiveSetup.probeTransform.rotation.y = 825;
event.receiveSetup.probeTransform.rotation.z = 733;
event.receiveSetup.probeTransform.translation.x = 54.72;
event.receiveSetup.probeTransform.translation.y = 45.83;
event.receiveSetup.probeTransform.translation.z = 86.52;
event.receiveSetup.samplingFrequency = 1275468;
event.receiveSetup.numberSamples = 48452;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(1));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(5753));
channelMappingI.pushBack(uint32(58));
channelMappingI.pushBack(uint32(60));
channelMappingI.pushBack(uint32(2));
channelMappingI.pushBack(uint32(1));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(4));
channelMappingI.pushBack(uint32(57));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(76));
channelMappingI.pushBack(uint32(21));
channelMappingI.pushBack(uint32(87));
channelMappingI.pushBack(uint32(9));
channelMappingI.pushBack(uint32(3));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(17));
channelMappingI.pushBack(uint32(26));
channelMappingI.pushBack(uint32(57));
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(8));
channelMappingI.pushBack(uint32(3));
channelMappingI.pushBack(uint32(6));
channelMappingI.pushBack(uint32(5));
event.receiveSetup.channelMapping.pushBack(channelMappingI);

event.receiveSetup.tgcProfile.pushBack(1.2);
event.receiveSetup.tgcProfile.pushBack(45.);
event.receiveSetup.tgcProfile.pushBack(4.25);
event.receiveSetup.tgcProfile.pushBack(-4.52);
event.receiveSetup.tgcProfile.pushBack(1);
event.receiveSetup.tgcProfile.pushBack(.5);
event.receiveSetup.tgcProfile.pushBack(4.36);
event.receiveSetup.tgcProfile.pushBack(1);
event.receiveSetup.tgcProfile.pushBack(2);
event.receiveSetup.tgcSamplingFrequency = -12.5;
event.receiveSetup.modulationFrequency = 1925;
event.receiveSetup.timeOffset = 206;
group.sequence.pushBack(event);

event = urx.Event();
event.transmitSetup.probe = dataset.acquisition.probes.data(1);
event.transmitSetup.probeTransform.rotation.x = 5;
event.transmitSetup.probeTransform.rotation.y = 8;
event.transmitSetup.probeTransform.rotation.z = 7;
event.transmitSetup.probeTransform.translation.x = 52;
event.transmitSetup.probeTransform.translation.y = 43;
event.transmitSetup.probeTransform.translation.z = 82;
event.transmitSetup.timeOffset = 1202;
event.transmitSetup.wave = dataset.acquisition.waves.data(2);
event.receiveSetup.probe = dataset.acquisition.probes.data(2);
event.receiveSetup.probeTransform.rotation.x = 55;
event.receiveSetup.probeTransform.rotation.y = 38;
event.receiveSetup.probeTransform.rotation.z = 33;
event.receiveSetup.probeTransform.translation.x = 2;
event.receiveSetup.probeTransform.translation.y = 4;
event.receiveSetup.probeTransform.translation.z = 2;
event.receiveSetup.samplingFrequency = 12468;
event.receiveSetup.numberSamples = 442;

channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(15));
channelMappingI.pushBack(uint32(32));
channelMappingI.pushBack(uint32(3));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(4));
channelMappingI.pushBack(uint32(55));
channelMappingI.pushBack(uint32(6));
event.receiveSetup.channelMapping.pushBack(channelMappingI);
channelMappingI = urx.StdVector('uint32', 1, urx.StdVectorType.RAW);
channelMappingI.pushBack(uint32(7));
channelMappingI.pushBack(uint32(48));
channelMappingI.pushBack(uint32(9));
event.receiveSetup.channelMapping.pushBack(channelMappingI);

event.receiveSetup.tgcProfile.pushBack(17);
event.receiveSetup.tgcProfile.pushBack(.2);
event.receiveSetup.tgcProfile.pushBack(3.);
event.receiveSetup.tgcProfile.pushBack(5);
event.receiveSetup.tgcProfile.pushBack(.1);
event.receiveSetup.tgcProfile.pushBack(35);
event.receiveSetup.tgcProfile.pushBack(.445);
event.receiveSetup.tgcProfile.pushBack(5);
event.receiveSetup.tgcSamplingFrequency = 47.5;
event.receiveSetup.modulationFrequency = 4.558;
event.receiveSetup.timeOffset = 7.38;
group.sequence.pushBack(event);

group.description = 'group2 description';
dataset.acquisition.groups.pushBack(group);


dataset.acquisition.timestamp = 1242;



groupData = urx.GroupData();
groupData.group = dataset.acquisition.groups.data(2);

%group_data->raw_data = std::make_shared<RawDataNoInit<double>>(6);
%double* buf = static_cast<double*>(group_data->raw_data->getBuffer());
%buf[0] = 1.2;
%buf[1] = 2.3;
%buf[2] = 3.4;
%buf[3] = 4.5;
%buf[4] = 5.6;
%buf[5] = 6.7;

groupData.groupTimestamp = 283954.334;
groupData.sequenceTimestamps.pushBack(1);
groupData.sequenceTimestamps.pushBack(2);
groupData.sequenceTimestamps.pushBack(4.5);
groupData.sequenceTimestamps.pushBack(1);
groupData.sequenceTimestamps.pushBack(.5);
groupData.sequenceTimestamps.pushBack(5.6);

eventTimestampI = urx.StdVector('double', 1, urx.StdVectorType.RAW);
eventTimestampI.pushBack(1);
eventTimestampI.pushBack(.24);
eventTimestampI.pushBack(1);
eventTimestampI.pushBack(5.2);
eventTimestampI.pushBack(4.5);
eventTimestampI.pushBack(7);
eventTimestampI.pushBack(.964);
eventTimestampI.pushBack(.5);
groupData.eventTimestamps.pushBack(eventTimestampI);
eventTimestampI = urx.StdVector('double', 1, urx.StdVectorType.RAW);
eventTimestampI.pushBack(1);
eventTimestampI.pushBack(2);
eventTimestampI.pushBack(4);
eventTimestampI.pushBack(85);
eventTimestampI.pushBack(.15);
eventTimestampI.pushBack(-4.5);
eventTimestampI.pushBack(-7);
eventTimestampI.pushBack(.45);
eventTimestampI.pushBack(.6);
eventTimestampI.pushBack(4);
groupData.eventTimestamps.pushBack(eventTimestampI);
dataset.acquisition.groupsData.pushBack(groupData);







groupData = urx.GroupData();
groupData.group = dataset.acquisition.groups.data(1);

%group_data->raw_data = std::make_shared<RawDataNoInit<double>>(6);
%double* buf = static_cast<double*>(group_data->raw_data->getBuffer());
%buf[0] = 1.2;
%buf[1] = 2.3;
%buf[2] = 3.4;
%buf[3] = 4.5;
%buf[4] = 5.6;
%buf[5] = 6.7;

groupData.groupTimestamp = 123;
groupData.sequenceTimestamps.pushBack(1);
groupData.sequenceTimestamps.pushBack(2);
groupData.sequenceTimestamps.pushBack(34);

eventTimestampI = urx.StdVector('double', 1, urx.StdVectorType.RAW);
eventTimestampI.pushBack(4);
eventTimestampI.pushBack(5);
eventTimestampI.pushBack(7);
groupData.eventTimestamps.pushBack(eventTimestampI);
eventTimestampI = urx.StdVector('double', 1, urx.StdVectorType.RAW);
eventTimestampI.pushBack(8);
eventTimestampI.pushBack(7);
eventTimestampI.pushBack(6);
groupData.eventTimestamps.pushBack(eventTimestampI);
dataset.acquisition.groupsData.pushBack(groupData);








acqObj = urx.Acquisition();

acqObj.localTime = 'coucou';
ptr = urx.LibBinding.getInstance().call('urx_Acquisition_shared_local_time', acqObj.id);
ptr_val = urx.LibBinding.getInstance().call('std_string_get', ptr);
assert(strcmp(ptr_val, 'coucou'))
urx.LibBinding.getInstance().call('std_string_set', ptr, 'foobar');
assert(strcmp('foobar', acqObj.localTime))

acqObj.soundSpeed = 1500;
ptr = urx.LibBinding.getInstance().call('urx_Acquisition_shared_sound_speed', acqObj.id);
ptr.setdatatype('doublePtr', 1)
assert(ptr.Value == 1500)
ptr.Value = 1540;
assert(1540 == acqObj.soundSpeed)
% return

g = urx.Group();
g.description = 'my description 0';
g1 = urx.Group();
g1.description = 'my description 1';
% return
acqObj.groups = [g g1];
assert(acqObj.groups(1) == g)
assert(acqObj.groups(2) == g1)
% return;


ev = urx.Event();
ev1 = urx.Event();
g.sequence = [ev ev1];
assert(g.sequence(1) == ev)
assert(g.sequence(2) == ev1)

rxs = urx.ReceiveSetup();
rxs1 = urx.ReceiveSetup();
%acqObj.uniquereceiveSetups = [rxs rxs1];
ev.receiveSetup = rxs;







