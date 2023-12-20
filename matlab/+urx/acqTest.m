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
%acqObj.uniqueReceive_setups = [rxs rxs1];
ev.receiveSetup = rxs;







