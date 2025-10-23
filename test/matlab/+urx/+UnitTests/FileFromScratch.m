classdef FileFromScratch < matlab.unittest.TestCase
  properties (MethodSetupParameter)
    libraryPath = {''}
    headerPath = {''}
    includepaths = {''}
    envpaths = {''}
  end

  methods(TestMethodSetup)
    function initTests(~, libraryPath, headerPath, includepaths, envpaths)
      feature('DefaultCharacterSet','UTF-8');

      urx.LibBinding.getInstance(libraryPath, headerPath, includepaths, envpaths);
    end
  end

  methods(Test)
    function updateVectorOfRawType(testcase)
      acquisition = urx.Acquisition();
      groupDataShared = urx.GroupData(); % Shared
      groupDataShared.groupTimestamp = 1.0;
      acquisition.groupsData = [groupDataShared]; % Raw

      groupDataShared2 = urx.GroupData(); % Shared
      groupDataShared2.groupTimestamp = 2.0;
      acquisition.groupsData(end+1) = groupDataShared2;

      testcase.verifyTrue(isempty(lastwarn));
      lastwarn('');
      warning('off');
      groupDataShared.groupTimestamp;
      testcase.verifyTrue(contains(lastwarn, 'urx.Acquisition.groupsData'));
      testcase.verifyTrue(contains(lastwarn, 'reallocated'));
      lastwarn('');
      warning('on');

      testcase.verifyTrue(isempty(lastwarn));
      lastwarn('');
      warning('off');
      testcase.verifyEqual(groupDataShared2.groupTimestamp, 2.0);
      testcase.verifyTrue(isempty(lastwarn));
      lastwarn('');
      warning('on');

      groupDataPtr = acquisition.groupsData(1); % Raw
      testcase.verifyEqual(groupDataPtr.groupTimestamp, 1.0);
      groupDataPtr2 = acquisition.groupsData(1); % Raw
      testcase.verifyEqual(groupDataPtr2.groupTimestamp, 1.0);

      acquisition.groupsData = [groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr, groupDataPtr]; % Raw
      % groupDataPtr points to the last element.
      % groupsData has realloc memory.

      testcase.verifyEqual(groupDataPtr.groupTimestamp, 1.0);

      acquisition.groupsData(1).groupTimestamp = 2.0;
      testcase.verifyEqual(groupDataPtr.groupTimestamp, 1.0);

      acquisition.groupsData(end).groupTimestamp = 3.0;
      testcase.verifyEqual(groupDataPtr.groupTimestamp, 3.0);

      testcase.verifyTrue(isempty(lastwarn));
      lastwarn('');
      warning('off');
      groupDataPtr2.groupTimestamp;
      testcase.verifyTrue(contains(lastwarn, 'urx.Acquisition.groupsData'));
      testcase.verifyTrue(contains(lastwarn, 'reallocated'));
      lastwarn('');
      warning('on');
    end

    function createFileFromScratch(testcase)
      libBinding = urx.LibBinding.getInstance();

      dataset = urx.Dataset();
      
      dataset.version.minor = 123;
      testcase.verifyEqual(dataset.version.minor, uint16(123));
      dataset.version.major = 456;
      dataset.version.patch = 789;
      
      version = urx.Version();
      version.minor = 111;
      version.major = 852;
      version.patch = 963;
      dataset.version = version;
      
      version2 = urx.Version();
      version2.minor = 111;
      version2.major = 852;
      version2.patch = 963;
      testcase.verifyFalse(version == version2);
      testcase.verifyTrue(isequal(version, version2));
      testcase.verifyEqual(version, version2);
      
      testcase.verifyEqual(dataset.version.minor, uint16(111));
      testcase.verifyEqual(dataset.version.major, uint16(852));
      testcase.verifyEqual(dataset.version.patch, uint16(963));
      version.minor = 1;
      version.major = 42;
      version.patch = 73;
      testcase.verifyEqual(dataset.version.minor, uint16(1));
      testcase.verifyEqual(dataset.version.major, uint16(42));
      testcase.verifyEqual(dataset.version.patch, uint16(73));
      dataset.version.minor = urx.Version().minor;
      dataset.version.major = urx.Version().major;
      dataset.version.patch = urx.Version().patch;
      testcase.verifyEqual(version.minor, uint16(urx.Version().minor));
      testcase.verifyEqual(version.major, uint16(urx.Version().major));
      testcase.verifyEqual(version.patch, uint16(urx.Version().patch));
      
      
      dataset.acquisition.authors = "AuthOr";
      dataset.acquisition.description = '康Description🐜';
      testcase.verifyTrue(strcmp(dataset.acquisition.description, '康Description🐜'));
      dataset.acquisition.localTime = "2019-06-15T15:53:00";
      dataset.acquisition.countryCode = 'FR';
      dataset.acquisition.system = '???';
      dataset.acquisition.timestamp = NaN;
      
      probe1 = urx.Probe();
      probe1.description = 'Probe 1';
      probe1.type = urx.Probe.ProbeType.MATRIX;
      testcase.verifyEqual(probe1.type, urx.Probe.ProbeType.MATRIX);
      probe1.transform.rotation.x = -1;
      probe1.transform.rotation.y = 2.3;
      probe1.transform.rotation.z = 4e5;
      probe1.transform.translation.x = 1.2;
      probe1.transform.translation.y = -2.2;
      probe1.transform.translation.z = 232;
      
      elementGeometry1 = urx.ElementGeometry();
      vector1 = urx.Vector3D(15, 2.3, 543);
      testcase.verifyEqual(vector1.x, 15);
      testcase.verifyEqual(vector1.y, 2.3);
      testcase.verifyEqual(vector1.z, 543);
      
      vector2 = urx.Vector3D();
      vector2.x = 2.2;
      vector2.y = 1.4;
      vector2.z = 0.2;
      elementGeometry1.perimeter = [vector1, vector2];
      
      elementGeometry2= urx.ElementGeometry();
      vector3 = urx.Vector3D();
      vector3.x = 32.2;
      vector3.y = 31.4;
      vector3.z = 30.2;
      
      vector4 = urx.Vector3D();
      vector4.x = 1.2;
      vector4.y = 131.;
      vector4.z = 543;
      elementGeometry2.perimeter = [vector3, vector4];
      probe1.elementGeometries = [urx.ElementGeometry, elementGeometry1, elementGeometry2];
      testcase.verifyEqual(probe1.elementGeometries(2).perimeter(1).y,elementGeometry1.perimeter(1).y);
      testcase.verifyEqual(probe1.elementGeometries(3).perimeter(2).y,elementGeometry2.perimeter(2).y);
      elementGeometry1.perimeter(1).y=23;
      elementGeometry2.perimeter(2).y=2.4;
      testcase.verifyEqual(probe1.elementGeometries(2).perimeter(1).y,elementGeometry1.perimeter(1).y);
      testcase.verifyEqual(probe1.elementGeometries(3).perimeter(2).y,elementGeometry2.perimeter(2).y);
      
      impulseResponse1 = urx.ImpulseResponse();
      impulseResponse1.samplingFrequency = 20000001;
      impulseResponse1.data = [1.2, 1.3];
      testcase.verifyEqual(impulseResponse1.data, [1.2, 1.3]);
      impulseResponse1.data = [impulseResponse1.data, 1.4];
      testcase.verifyEqual(impulseResponse1.data, [1.2, 1.3, 1.4]);
      impulseResponse1.units = 'meter';
      impulseResponse1.timeOffset = 10000;
      
      impulseResponse2 = urx.ImpulseResponse();
      impulseResponse2.samplingFrequency = NaN;
      impulseResponse2.data = [2.2, 2.3, 2.4];
      impulseResponse2.units = 'milli';
      impulseResponse2.timeOffset = NaN;
      probe1.impulseResponses = [urx.ImpulseResponse, impulseResponse1, impulseResponse2];
      
      element1 = urx.Element();
      element1.transform.rotation.x = 1;
      element1.transform.rotation.y = 2;
      element1.transform.rotation.z = 3;
      element1.transform.translation.x = 4e-3;
      element1.transform.translation.y = 5e-3;
      element1.transform.translation.z = 6e-3;
      element1.elementGeometry = probe1.elementGeometries(1);
      element1.impulseResponse = probe1.impulseResponses(2);
      
      element2 = urx.Element();
      element2.transform.rotation.x = -1;
      element2.transform.rotation.y = -2;
      element2.transform.rotation.z = -3;
      element2.transform.translation.x = 4e-3;
      element2.transform.translation.y = 5e-3;
      element2.transform.translation.z = 6e-3;
      element2.elementGeometry = probe1.elementGeometries(2);
      testcase.verifyEqual(element2.elementGeometry.perimeter(1).x, 15);
      element2.impulseResponse = probe1.impulseResponses(3);
      
      element3 = urx.Element();
      element3.transform.rotation.x = 1.23;
      element3.transform.rotation.y = 4.56;
      element3.transform.rotation.z = 7.89;
      element3.transform.translation.x = 10;
      element3.transform.translation.y = -11.1;
      element3.transform.translation.z = 12.3;
      element3.elementGeometry = probe1.elementGeometries(2);
      element3.impulseResponse = probe1.impulseResponses(1);
      
      element4 = urx.Element();
      element4.transform.rotation.x = -33;
      element4.transform.rotation.y = 2.2;
      element4.transform.rotation.z = 44;
      element4.transform.translation.x = 77;
      element4.transform.translation.y = 8.8;
      element4.transform.translation.z = -97.8e5;
      element4.elementGeometry = probe1.elementGeometries(3);
      element4.impulseResponse = probe1.impulseResponses(2);
      
      probe1.elements = [urx.Element, element1, element2, element3, element4];
      
      probe2 = urx.Probe();
      probe2.description = 'Probe 2';
      probe2.type = 123;
      testcase.verifyEqual(probe2.type, int32(123));
      probe2.transform.rotation.x = 3;
      probe2.transform.rotation.y = 4;
      probe2.transform.rotation.z = 5;
      probe2.transform.translation.x = 3.2;
      probe2.transform.translation.y = 4.2;
      probe2.transform.translation.z = 5.2;
      
      elementGeometry1 = urx.ElementGeometry();
      vector1 = urx.Vector3D();
      vector1.x = 123;
      vector1.y = 4.5;
      vector1.z = 5.4;
      vector2 = urx.Vector3D();
      vector2.x = 1.1;
      vector2.y = 22.1;
      vector2.z = 5.4;
      elementGeometry1.perimeter = [vector1, vector2];
      
      elementGeometry2 = urx.ElementGeometry();
      vector1 = urx.Vector3D();
      vector1.x = 5.5;
      vector1.y = 6.6;
      vector1.z = 7.7;
      vector2 = urx.Vector3D();
      vector2.x = 1.1;
      vector2.y = 0.0;
      vector2.z = 5.5;
      elementGeometry2.perimeter = [vector1, vector2];

      probe2.elementGeometries = [urx.ElementGeometry, elementGeometry1, elementGeometry2];
      % TODO: Replace previous line by next following lines.
      %probe2.elementGeometries = elementGeometry1;
      %testcase.verifyEqual(probe2.elementGeometries(1).perimeter(1).y,elementGeometry1.perimeter(1).y);
      %elementGeometry1.perimeter(1).y=211.;
      %testcase.verifyEqual(probe2.elementGeometries(1).perimeter(1).y,elementGeometry1.perimeter(1).y);
      %probe2.elementGeometries = [probe2.elementGeometries, elementGeometry2];
      % For the current time, elementGeometry1 points to an invalid memory area because previous line
      % changes the size of std::vector. So it reallocates memory and change pointer memory location.
      %testcase.verifyEqual(probe2.elementGeometries(1).perimeter(1).y,elementGeometry1.perimeter(1).y);
      %testcase.verifyEqual(probe2.elementGeometries(2).perimeter(2).y,elementGeometry2.perimeter(2).y);
      %elementGeometry1.perimeter(1).y=311.;
      %elementGeometry2.perimeter(2).y=331.;
      %testcase.verifyEqual(probe2.elementGeometries(1).perimeter(1).y,elementGeometry1.perimeter(1).y);
      %testcase.verifyEqual(probe2.elementGeometries(2).perimeter(2).y,elementGeometry2.perimeter(2).y);
      
      impulseResponse1 = urx.ImpulseResponse();
      impulseResponse1.samplingFrequency = 20000011;
      impulseResponse1.data = [22.2, 3.3, 144.4];
      impulseResponse1.units = 'met55er';
      impulseResponse1.timeOffset = 100500;
      
      impulseResponse2 = urx.ImpulseResponse();
      impulseResponse2.samplingFrequency = 5000004402;
      impulseResponse2.data = [23.2, 2.53, 72.4];
      impulseResponse2.units = 'mi3lli';
      impulseResponse2.timeOffset = 200100;
      probe2.impulseResponses = [urx.ImpulseResponse, impulseResponse1, impulseResponse2];
      
      element1 = urx.Element();
      element1.transform.rotation.x = -99;
      element1.transform.rotation.y = 8.8;
      element1.transform.rotation.z = 77;
      element1.transform.translation.x = 74.1;
      element1.transform.translation.y = 8.52;
      element1.transform.translation.z = -.963;
      % Test empty weak_ptr field
      element1.elementGeometry = urx.ElementGeometry.empty;
      testcase.verifyFalse(libBinding.call('urx_Element_shared_element_geometry_has_data', element1));
      element1.elementGeometry = probe2.elementGeometries(2);
      testcase.verifyTrue(libBinding.call('urx_Element_shared_element_geometry_has_data', element1));
      element1.impulseResponse = probe2.impulseResponses(2);
      
      element2 = urx.Element();
      element2.transform.rotation.x = 3.3;
      element2.transform.rotation.y = 11;
      element2.transform.rotation.z = 2.2;
      element2.transform.translation.x = -86;
      element2.transform.translation.y = -75.3;
      element2.transform.translation.z = 42.;
      element2.elementGeometry = probe2.elementGeometries(3);
      element2.impulseResponse = probe2.impulseResponses(3);
      probe2.elements = [element1, element2];
      
      dataset.acquisition.probes = [urx.Probe, probe1, probe2];
      
      excitation1 = urx.Excitation();
      excitation1.pulseShape = 'sinusoidal';
      excitation1.transmitFrequency = 123456789;
      excitation1.samplingFrequency = 987654321;
      excitation1.waveform = [0, 0.1, 0.2];
      
      excitation2 = urx.Excitation();
      excitation2.pulseShape = 'square wave';
      excitation2.transmitFrequency = 741085296;
      excitation2.samplingFrequency = 369258147;
      excitation2.waveform = [1, 1.1, 1.2];
      dataset.acquisition.excitations = [urx.Excitation, excitation1, excitation2];
      
      group1 = urx.Group();
      group1.samplingType = urx.Group.SamplingType.IQ;
      group1.dataType = urx.Group.DataType.DOUBLE;
      group1.soundSpeed = 1500.;
      testcase.verifyEqual(group1.soundSpeed, 1500);
      
      event1 = urx.Event();
      event1.transmitSetup.probe = dataset.acquisition.probes(1);
      event1.transmitSetup.probeTransform.rotation.x = 5.1e-120;
      event1.transmitSetup.probeTransform.rotation.y = -8;
      event1.transmitSetup.probeTransform.rotation.z = 7;
      event1.transmitSetup.probeTransform.translation.x = 5.2;
      event1.transmitSetup.probeTransform.translation.y = 4.3;
      event1.transmitSetup.probeTransform.translation.z = 8.2e10;
      event1.transmitSetup.timeOffset = 120.2;
      event1.transmitSetup.wave.type = urx.Wave.WaveType.CONVERGING_WAVE;
      event1.transmitSetup.wave.timeZeroReferencePoint.x = 5;
      event1.transmitSetup.wave.timeZeroReferencePoint.y = 4;
      event1.transmitSetup.wave.timeZeroReferencePoint.z = 3;
      event1.transmitSetup.wave.timeZero = 5.11;
      event1.transmitSetup.wave.parameters = [7, 53e-12, .2, 1, .3, 5.6e20, 7];
      event1.transmitSetup.activeElements = {uint32([]), [uint32(1), uint32(2)], [uint32(1)]};
      event1.transmitSetup.excitations = [dataset.acquisition.excitations(2), dataset.acquisition.excitations(1)];
      testcase.verifyEqual(excitation1.waveform, dataset.acquisition.excitations(2).waveform);
      event1.transmitSetup.delays = [1.2, .3];
      
      event1.receiveSetup.probe = dataset.acquisition.probes(2);
      event1.receiveSetup.probeTransform.rotation.x = 5.1e-120;
      event1.receiveSetup.probeTransform.rotation.y = -8;
      event1.receiveSetup.probeTransform.rotation.z = 7;
      event1.receiveSetup.probeTransform.translation.x = 5.2;
      event1.receiveSetup.probeTransform.translation.y = 4.3;
      event1.receiveSetup.probeTransform.translation.z = 8.2e10;
      event1.receiveSetup.samplingFrequency = 125468;
      event1.receiveSetup.numberSamples = 1;
      event1.receiveSetup.activeElements = {[uint32(2), uint32(3), uint32(754), uint32(9), uint32(7), uint32(3), uint32(2)], uint32([])};
      event1.receiveSetup.tgcProfile = [1.2, 45e120, 4.5, -4.2e-50, 1, .5, 4.6, 1, 2];
      event1.receiveSetup.tgcSamplingFrequency = 12.5e10;
      event1.receiveSetup.modulationFrequency = -125;
      event1.receiveSetup.timeOffset = 1205;
      
      event2 = urx.Event();
      event2.transmitSetup.probe = dataset.acquisition.probes(2);
      event2.transmitSetup.probeTransform.rotation.x = 5.1e-120;
      event2.transmitSetup.probeTransform.rotation.y = -8;
      event2.transmitSetup.probeTransform.rotation.z = 7.;
      event2.transmitSetup.probeTransform.translation.x = 5.2;
      event2.transmitSetup.probeTransform.translation.y = 4.3;
      event2.transmitSetup.probeTransform.translation.z = 8.2e10;
      event2.transmitSetup.timeOffset = 12052;
      event2.transmitSetup.wave.type = urx.Wave.WaveType.CONVERGING_WAVE;
      event2.transmitSetup.wave.timeZeroReferencePoint.x = 1;
      event2.transmitSetup.wave.timeZeroReferencePoint.y = 2;
      event2.transmitSetup.wave.timeZeroReferencePoint.z = 3;
      event2.transmitSetup.wave.timeZero = 4.11;
      event2.transmitSetup.wave.parameters = [7, .3, 5.6, 7];
      event2.transmitSetup.activeElements = {[uint32(2)], [uint32(1), uint32(2)]};
      event2.transmitSetup.excitations = [dataset.acquisition.excitations(2), dataset.acquisition.excitations(2)];
      event2.transmitSetup.delays = [.3, 1.2];
      
      event2.receiveSetup.probe = dataset.acquisition.probes(3);
      event2.receiveSetup.probeTransform.rotation.x = 5.1e-120;
      event2.receiveSetup.probeTransform.rotation.y = -8;
      event2.receiveSetup.probeTransform.rotation.z = 7.;
      event2.receiveSetup.probeTransform.translation.x = 5.2;
      event2.receiveSetup.probeTransform.translation.y = 4.3;
      event2.receiveSetup.probeTransform.translation.z = 8.2e10;
      event2.receiveSetup.samplingFrequency = 1268;
      event2.receiveSetup.numberSamples = 2;
      event2.receiveSetup.activeElements = {[uint32(2), uint32(3), uint32(4)]};
      event2.receiveSetup.tgcProfile = [1, .2, 4., 5, .1, 5, .45, 5];
      event2.receiveSetup.tgcSamplingFrequency = 4.5;
      event2.receiveSetup.modulationFrequency = 4.58;
      event2.receiveSetup.timeOffset = 7.8;
      
      group1.sequence = [urx.Event, event1, event2];
      group1.description = 'group description';
      
      
      group2 = urx.Group();
      group2.samplingType = urx.Group.SamplingType.RF;
      group2.dataType = urx.Group.DataType.INT16;
      group2.soundSpeed = 1550.;
      
      event1 = urx.Event();
      event1.transmitSetup.probe = dataset.acquisition.probes(2);
      event1.transmitSetup.probeTransform.rotation.x = 5.1e-120;
      event1.transmitSetup.probeTransform.rotation.y = -8;
      event1.transmitSetup.probeTransform.rotation.z = 7.;
      event1.transmitSetup.probeTransform.translation.x = 5.2;
      event1.transmitSetup.probeTransform.translation.y = 4.3;
      event1.transmitSetup.probeTransform.translation.z = 8.2e10;
      event1.transmitSetup.timeOffset = 1202.2;
      event1.transmitSetup.wave.type = urx.Wave.WaveType.CYLINDRICAL_WAVE;
      event1.transmitSetup.wave.timeZeroReferencePoint.x = 3;
      event1.transmitSetup.wave.timeZeroReferencePoint.y = 4;
      event1.transmitSetup.wave.timeZeroReferencePoint.z = 5;
      event1.transmitSetup.wave.timeZero = 5.161;
      event1.transmitSetup.wave.parameters = [1, .2, 4, 3, 6, 4.5];
      event1.transmitSetup.activeElements = {[uint32(1)], [uint32(2)]};
      event1.transmitSetup.excitations = [dataset.acquisition.excitations(1), dataset.acquisition.excitations(2)];
      event1.transmitSetup.delays = [1.2, 45];
      
      event1.receiveSetup.probe = dataset.acquisition.probes(1);
      event1.receiveSetup.probeTransform.rotation.x = 5.1e-120;
      event1.receiveSetup.probeTransform.rotation.y = -8;
      event1.receiveSetup.probeTransform.rotation.z = 7;
      event1.receiveSetup.probeTransform.translation.x = 5.2;
      event1.receiveSetup.probeTransform.translation.y = 4.3;
      event1.receiveSetup.probeTransform.translation.z = 8.2e10;
      event1.receiveSetup.samplingFrequency = 1275468;
      event1.receiveSetup.numberSamples = 2;
      event1.receiveSetup.activeElements = {[uint32(2), uint32(3), uint32(5754), uint32(59), uint32(61), uint32(3), uint32(2)]};
      event1.receiveSetup.tgcProfile = [1.2, 45., 4.25, -4.52, 1, .5, 4.36, 1, 2];
      event1.receiveSetup.tgcSamplingFrequency = -12.5;
      event1.receiveSetup.modulationFrequency = 1925;
      event1.receiveSetup.timeOffset = 206;
      
      event2 = urx.Event();
      event2.transmitSetup.probe = dataset.acquisition.probes(1);
      event2.transmitSetup.probeTransform.rotation.x = 5.1e-120;
      event2.transmitSetup.probeTransform.rotation.y = -8;
      event2.transmitSetup.probeTransform.rotation.z = 7;
      event2.transmitSetup.probeTransform.translation.x = 5.2;
      event2.transmitSetup.probeTransform.translation.y = 4.3;
      event2.transmitSetup.probeTransform.translation.z = 8.2e10;
      event2.transmitSetup.timeOffset = 1202;
      event2.transmitSetup.wave.type = urx.Wave.WaveType.CONVERGING_WAVE;
      event2.transmitSetup.wave.timeZeroReferencePoint.x = 6;
      event2.transmitSetup.wave.timeZeroReferencePoint.y = 5;
      event2.transmitSetup.wave.timeZeroReferencePoint.z = 2;
      event2.transmitSetup.wave.timeZero = 4.11;
      event2.transmitSetup.wave.parameters = [7, 53, .3, 5.6, 7];
      event2.transmitSetup.activeElements = {[uint32(2)]};
      event2.transmitSetup.excitations = [dataset.acquisition.excitations(1)];
      event2.transmitSetup.delays = [1.6];
      
      event2.receiveSetup.probe = dataset.acquisition.probes(3);
      event2.receiveSetup.probeTransform.rotation.x = 5.1e-120;
      event2.receiveSetup.probeTransform.rotation.y = -8;
      event2.receiveSetup.probeTransform.rotation.z = 7;
      event2.receiveSetup.probeTransform.translation.x = 5.2;
      event2.receiveSetup.probeTransform.translation.y = 4.3;
      event2.receiveSetup.probeTransform.translation.z = 8.2e10;
      event2.receiveSetup.samplingFrequency = 12468;
      event2.receiveSetup.numberSamples = 2;
      event2.receiveSetup.activeElements = {[uint32(16), uint32(33), uint32(4)]};
      event2.receiveSetup.tgcProfile = [17, .2, 3., 5, .1, 35, .445, 5];
      event2.receiveSetup.tgcSamplingFrequency = 47.5;
      event2.receiveSetup.modulationFrequency = 4.558;
      event2.receiveSetup.timeOffset = 7.38;
      group2.sequence = [urx.Event, event1, event2];
      
      group2.description = 'group2 description';
      dataset.acquisition.groups = [urx.Group, group1, group2, urx.Group];
      dataset.acquisition.groups(end).sequence = [urx.Event];
      testcase.verifyTrue(dataset.acquisition.groups(1) == dataset.acquisition.groups(1));
      dataset.acquisition.timestamp = 1242;
      
      
      groupData1 = urx.GroupData();
      groupData1.group = dataset.acquisition.groups(3);
      rawData1 = urx.RawData_double_real(6);
      testcase.verifyEqual(rawData1.size, int64(6));
      testcase.verifyEqual(rawData1.dataType(), urx.Group.DataType.DOUBLE);
      testcase.verifyEqual(rawData1.samplingType(), urx.Group.SamplingType.RF);
      rawData1.data(1,1) = 1.2;
      rawData1.data(1,2) = 2.3;
      rawData1.data(1,3) = 3.4;
      rawData1.data(1,4) = 4.5;
      rawData1.data(1,5) = 5.6;
      rawData1.data(1,6) = 6.7;
      groupData1.rawData = rawData1;
      groupData1.groupTimestamp = 283954.334;
      groupData1.sequenceTimestamps = [1, 2, 4.2, 1, .5, 5.6];
      groupData1.eventTimestamps = {[1, .24, 1, 5.2, 4.5, 7, .964, .5], [1, 2, 4, 85, .15, -4.5, -7, .45, .6, 4]};
      
      
      groupData2 = urx.GroupData();
      groupData2.group = dataset.acquisition.groups(2);
      rawData2 = urx.RawData_int16_t_complex(4);
      testcase.verifyEqual(rawData2.size, int64(4));
      testcase.verifyEqual(rawData2.dataType(), urx.Group.DataType.INT16);
      testcase.verifyEqual(rawData2.samplingType(), urx.Group.SamplingType.IQ);
      rawData2.data(1,1) = 123;
      rawData2.data(2,1) = 456;
      rawData2.data(1,2) = 159;
      rawData2.data(2,2) = 753;
      rawData2.data(1,3) = 789;
      rawData2.data(2,3) = 456;
      rawData2.data(1,4) = 123;
      rawData2.data(2,4) = 753;
      groupData2.rawData = rawData2;
      groupData2.groupTimestamp = 123;
      groupData2.sequenceTimestamps = [1, 2, 34];
      groupData2.eventTimestamps = {[4, 5, 7], [8, 7, 6]};
      dataset.acquisition.groupsData = [urx.GroupData, groupData1, groupData2];

      groupData3 = urx.GroupData();
      groupData3.group = dataset.acquisition.groups(2);
      rawData3 = urx.RawData_int16_t_complex(5);
      rawData3.data(1,1) = 123;
      rawData3.data(2,1) = 456;
      rawData3.data(1,2) = 159;
      rawData3.data(2,2) = 753;
      rawData3.data(1,3) = 789;
      rawData3.data(2,3) = 456;
      rawData3.data(1,4) = 123;
      rawData3.data(2,4) = 753;
      rawData3.data(1,5) = -255;
      rawData3.data(2,5) = 15909;
      groupData3.rawData = rawData3;

      groupData4 = urx.GroupData();
      groupData4.group = dataset.acquisition.groups(1);
      rawData4 = urx.RawData_int32_t_real(0);
      groupData4.rawData = rawData4;

      groupData5 = urx.GroupData();
      rawData5 = urx.RawData_int16_t_real(1);
      groupData5.rawData = rawData5;
      rawData5.data(1,1) = 123;

      groupData6 = urx.GroupData();
      rawData6 = urx.RawData_int32_t_real(1);
      groupData6.rawData = rawData6;
      rawData6.data(1,1) = 2345678;

      groupData7 = urx.GroupData();
      rawData7 = urx.RawData_float_real(1);
      groupData7.rawData = rawData7;
      rawData7.data(1,1) = 2345.678;

      groupData8 = urx.GroupData();
      rawData8 = urx.RawData_double_real(1);
      groupData8.rawData = rawData8;
      rawData8.data(1,1) = 234567.8;

      groupData9 = urx.GroupData();
      rawData9 = urx.RawData_int16_t_complex(1);
      groupData9.rawData = rawData9;
      rawData9.data(1,1) = 123;
      rawData9.data(2,1) = -123;

      groupData10 = urx.GroupData();
      rawData10 = urx.RawData_int32_t_complex(1);
      groupData10.rawData = rawData10;
      rawData10.data(1,1) = 2345678;
      rawData10.data(2,1) = -2345678;

      groupData11 = urx.GroupData();
      rawData11 = urx.RawData_float_complex(1);
      groupData11.rawData = rawData11;
      rawData11.data(1,1) = 2.;
      rawData11.data(2,1) = 345.678;

      groupData12 = urx.GroupData();
      rawData12 = urx.RawData_double_complex(1);
      groupData12.rawData = rawData12;
      rawData12.data(1,1) = 234.;
      rawData12.data(2,1) = 567.8;

      dataset.acquisition.groupsData = [urx.GroupData, groupData1, groupData2, groupData3, groupData4, groupData5, groupData6, groupData7, groupData8, groupData9, groupData10, groupData11, groupData12];

      urx.saveToFile('test-matlab.urx', dataset);

      dataset2 = urx.loadFromFile('test-matlab.urx');
      
      testcase.verifyTrue(isequal(dataset, dataset2));
      testcase.verifyEqual(dataset.version.minor, dataset2.version.minor);

      testcase.verifyEqual(dataset.acquisition, dataset2.acquisition);
      testcase.verifyEqual(dataset, dataset2);
      testcase.verifyEqual(dataset.acquisition.probes(2).elements(2).elementGeometry, ...
        dataset2.acquisition.probes(2).elements(2).elementGeometry);
      testcase.verifyEqual(dataset.acquisition.probes(2).elements(2), ...
        dataset2.acquisition.probes(2).elements(2));
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2), ...
        dataset2.acquisition.groups(2).sequence(2));
      testcase.verifyEqual(dataset.acquisition.excitations(2), dataset2.acquisition.excitations(2));
      testcase.verifyEqual(dataset.acquisition.groupsData(2), dataset2.acquisition.groupsData(2));
      testcase.verifyEqual(dataset.acquisition.groups(2), dataset2.acquisition.groups(2));
      testcase.verifyEqual(dataset.acquisition.probes(2).elements(2).impulseResponse, ...
        dataset2.acquisition.probes(2).elements(2).impulseResponse);
      testcase.verifyEqual(dataset.acquisition.probes(2), dataset2.acquisition.probes(2));
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2).receiveSetup, ...
        dataset2.acquisition.groups(2).sequence(2).receiveSetup);
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2).receiveSetup.probeTransform, ...
        dataset2.acquisition.groups(2).sequence(2).receiveSetup.probeTransform);
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2).transmitSetup, ...
        dataset2.acquisition.groups(2).sequence(2).transmitSetup);
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2).receiveSetup.probeTransform.rotation, ...
        dataset2.acquisition.groups(2).sequence(2).receiveSetup.probeTransform.rotation);
      testcase.verifyEqual(dataset.version, dataset2.version);
      testcase.verifyEqual(dataset.acquisition.groups(2).sequence(2).transmitSetup.wave, ...
        dataset2.acquisition.groups(2).sequence(2).transmitSetup.wave);

      testcase.verifyError(@() dataset.isequal(dataset2.acquisition), 'urx:fatalError');
      
      datasetCloned = urx.cloneDataset(dataset);

      testcase.verifyEqual(dataset, datasetCloned);
      testcase.verifyEqual(dataset.acquisition.authors, datasetCloned.acquisition.authors);
      testcase.verifyEqual(dataset.acquisition.description, datasetCloned.acquisition.description);
      datasetCloned.acquisition.authors = "Other authors";
      testcase.verifyNotEqual(dataset.acquisition.authors, datasetCloned.acquisition.authors);
      testcase.verifyEqual(dataset.acquisition.description, datasetCloned.acquisition.description);

      stream_filename = "stream.urx";

      raw_data_vector_double = urx.RawData_double_complex(4);
      raw_data_vector_double.data(1,1) = 1.2;
      raw_data_vector_double.data(1,2) = 3.4;
      raw_data_vector_double.data(1,3) = -5.6;
      raw_data_vector_double.data(1,4) = 5.6;
      raw_data_vector_double.data(2,1) = 21.2;
      raw_data_vector_double.data(2,2) = 24.56;
      raw_data_vector_double.data(2,3) = -2.12;
      raw_data_vector_double.data(2,4) = -2.456;

      raw_data_vector_double2 = urx.RawData_double_complex(4);
      raw_data_vector_double2.data(1,1) = 12.;
      raw_data_vector_double2.data(1,2) = 34.;
      raw_data_vector_double2.data(1,3) = -56;
      raw_data_vector_double2.data(1,4) = 15.3;
      raw_data_vector_double2.data(2,1) = 29.87;
      raw_data_vector_double2.data(2,2) = 26.54;
      raw_data_vector_double2.data(2,3) = 23.12;
      raw_data_vector_double2.data(2,4) = -2.312;

      raw_data_vector_short = urx.RawData_int16_t_real(4);
      raw_data_vector_short.data(1,1) = 12;
      raw_data_vector_short.data(1,2) = 456;
      raw_data_vector_short.data(1,3) = -12;
      raw_data_vector_short.data(1,4) = -456;

      raw_data_vector_short2 = urx.RawData_int16_t_real(4);
      raw_data_vector_short2.data(1,1) = 987;
      raw_data_vector_short2.data(1,2) = 654;
      raw_data_vector_short2.data(1,3) = 312;
      raw_data_vector_short2.data(1,4) = -312;

      raw_data_double = urx.RawData_double_complex(8);
      raw_data_double.data(1,1) = 1.2;
      raw_data_double.data(1,2) = 3.4;
      raw_data_double.data(1,3) = -5.6;
      raw_data_double.data(1,4) = 5.6;
      raw_data_double.data(1,5) = 12.;
      raw_data_double.data(1,6) = 34.;
      raw_data_double.data(1,7) = -56;
      raw_data_double.data(1,8) = 15.3;
      raw_data_double.data(2,1) = 21.2;
      raw_data_double.data(2,2) = 24.56;
      raw_data_double.data(2,3) = -2.12;
      raw_data_double.data(2,4) = -2.456;
      raw_data_double.data(2,5) = 29.87;
      raw_data_double.data(2,6) = 26.54;
      raw_data_double.data(2,7) = 23.12;
      raw_data_double.data(2,8) = -2.312;

      raw_data_short = urx.RawData_int16_t_real(8);
      raw_data_short.data(1,1) = 12;
      raw_data_short.data(1,2) = 456;
      raw_data_short.data(1,3) = -12;
      raw_data_short.data(1,4) = -456;
      raw_data_short.data(1,5) = 987;
      raw_data_short.data(1,6) = 654;
      raw_data_short.data(1,7) = 312;
      raw_data_short.data(1,8) = -312;

      % Write using stream
      stream = urx.Stream(stream_filename, dataset);
      % Need to save to have a valid dataset file to write groups data inside
      stream.saveToFile();
      stream.setWriterOptions(false, false, false);
      group_data_stream1 = stream.createGroupData(dataset.acquisition.groups(3), 1.);
      group_data_stream1.append(raw_data_vector_short, 1.2, [2.3, 3.4, 4.5]);
      group_data_stream1.append(raw_data_vector_short2, 12, [23., 34., 45.]);
      stream.setWriterOptions(true, false, false);
      group_data_stream2 = stream.createGroupData(dataset.acquisition.groups(2), 2.);
      group_data_stream2.append(raw_data_vector_double, 9., [8.]);
      group_data_stream2.append(raw_data_vector_double2, 5., [8., 9.4]);

      % Need to clear to release handle to file "stream_filename".
      clear group_data_stream1;
      clear group_data_stream2;

      % Full read
      dataset_loaded = urx.loadFromFile(stream_filename);

      group_data_short = dataset_loaded.acquisition.groupsData(numel(dataset_loaded.acquisition.groupsData) - 1);
      testcase.verifyEqual(group_data_short.sequenceTimestamps, [1.2, 12]);
      testcase.verifyEqual(group_data_short.eventTimestamps, {[2.3, 3.4, 4.5], [23., 34., 45.]});
      testcase.verifyTrue(isequaln(group_data_short.rawData, raw_data_short));

      group_data_double = dataset_loaded.acquisition.groupsData(numel(dataset_loaded.acquisition.groupsData));
      testcase.verifyEqual(group_data_double.sequenceTimestamps, [9., 5.]);
      testcase.verifyEqual(group_data_double.eventTimestamps, {[8.], [8., 9.4]});
      testcase.verifyTrue(isequaln(group_data_double.rawData, raw_data_double));

      clear dataset_loaded;
      clear stream;

      dataset_loaded = urx.Dataset;
      stream = urx.Stream(stream_filename, dataset_loaded);
      stream.setReaderOptions(urx.RawDataLoadPolicy.STREAM);
      stream.loadFromFile();

      buffer_short = urx.RawData_int16_t_real(8);
      stream.readRawData(numel(dataset_loaded.acquisition.groupsData)-1, buffer_short, 1, 1, 1);
      stream.readRawData(numel(dataset_loaded.acquisition.groupsData)-1, buffer_short, 2, 2, 1);
      testcase.verifyTrue(isequaln(buffer_short, raw_data_short));

      buffer_double = urx.RawData_double_complex(8);
      stream.readRawData(numel(dataset_loaded.acquisition.groupsData), buffer_double, 1, 1, 1);
      stream.readRawData(numel(dataset_loaded.acquisition.groupsData), buffer_double, 2, 2, 1);
      testcase.verifyTrue(isequaln(buffer_double, raw_data_double));

      clear stream;

      delete(stream_filename);
    end


    function readRawData(testcase)
      Nelements = 5;
      dataset = urx.Dataset();
      acq = dataset.acquisition;
      acq.groupsData = urx.GroupData();
      acq.groupsData.rawData = urx.RawData_double_real(Nelements);
  
      for i = 1:Nelements
        acq.groupsData.rawData.data(i) = i;
      end
  
      urx.saveToFile("test-rawdata.urx", dataset);

      dataset2 = urx.loadFromFile('test-rawdata.urx');

      testcase.verifyEqual(dataset.acquisition.groupsData.rawData.size, dataset2.acquisition.groupsData.rawData.size)
      testcase.verifyEqual(dataset.acquisition.groupsData.rawData.data, dataset2.acquisition.groupsData.rawData.data)
    end

    function readVectorVector(testcase)
      dataset = urx.Dataset();
      acq = dataset.acquisition;
      acq.groupsData = urx.GroupData();
      acq.groups = urx.Group();
      acq.groups.sequence = urx.Event();
      actives = cell(1,1);
      actives{1} = uint32(43);
      rs = acq.groups(1).sequence(1).receiveSetup;
      rs.activeElements = actives;
      urx.saveToFile('test-vector-vector.urx', dataset, false, false, false);

      testcase.verifyFalse(urx.validate(dataset))

      dataset2 = urx.loadFromFile('test-vector-vector.urx', urx.RawDataLoadPolicy.STREAM);
      dataset3 = urx.loadFromFile('test-vector-vector.urx', urx.RawDataLoadPolicy.FULL);

      rs2 = dataset2.acquisition.groups(1).sequence(1).receiveSetup;
      rs3 = dataset3.acquisition.groups(1).sequence(1).receiveSetup;
      testcase.verifyEqual(actives, rs2.activeElements);
      testcase.verifyEqual(actives, rs3.activeElements);
    end

    function assignSharedInWeak(testcase)
      a=urx.Element;
      b=urx.ElementGeometry;
      a.elementGeometry = b;
      b;
    end

    function assignFailure(testcase)
      groupData = urx.GroupData;
      testcase.verifyTrue(isempty(lastwarn));
      lastwarn('');
      warning('off');
      groupData.rawData = urx.RawData_int16_t_complex();
      testcase.verifyFalse(isempty(lastwarn));
      lastwarn('');
      warning('on');
      groupData.rawData = urx.RawData_int16_t_complex(10);
      groupData.rawData;
    end

    function vector3dSingle(testcase)
      A = urx.Vector3D(double(1),0,0);
      B = urx.ElementGeometry();
      B.perimeter = A;
      testcase.verifyTrue(isa(A.x, 'double'))
      testcase.verifyTrue(isa(B.perimeter.x, 'double'))
      testcase.verifyEqual(A.x, B.perimeter.x)

      C = urx.Vector3D(single(1),0,0);
      D = urx.ElementGeometry();
      D.perimeter = C;
      testcase.verifyTrue(isa(C.x, 'double'))
      testcase.verifyTrue(isa(D.perimeter.x, 'double'))
      testcase.verifyEqual(C.x, D.perimeter.x)
    end

    function incrementSharedArray(testcase)
      probe = urx.Probe();

      elementGeometry = urx.ElementGeometry();
      elementGeometry.perimeter = [urx.Vector3D(-1,-1,0),...
                              urx.Vector3D(1,-1,0),...
                              urx.Vector3D(1,1,0),...
                              urx.Vector3D(-1,1,0)];

      probe.elementGeometries = elementGeometry;

      probe.elements = urx.Element();
      probe.elements.elementGeometry = elementGeometry;
      probe.elements.transform.rotation.x = 1;

      for kk=2:10
        element = urx.Element();
        element.elementGeometry = elementGeometry;
        element.transform.rotation.x = kk;
        probe.elements = [probe.elements,element];
      end

      for kk=11:20
        element = urx.Element();
        element.elementGeometry = elementGeometry;
        element.transform.rotation.x = kk;
        probe.elements(end+1) = element;
      end

      for kk=2:10
        testcase.verifyEqual(numel(probe.elements(kk).elementGeometry.perimeter), 4);
        testcase.verifyEqual(probe.elements(kk).elementGeometry.perimeter(1).x, -1);
        testcase.verifyEqual(probe.elements(kk).transform.rotation.x, kk);
      end
    end

    function incrementRawArray(testcase)
      group = urx.Group();

      group.sequence = urx.Event();
      group.sequence.transmitSetup.delays = 1;

      for kk=2:10
        event = urx.Event();
        event.transmitSetup.delays = kk;
        group.sequence = [group.sequence,event];
      end

      for kk=11:20
        event = urx.Event();
        event.transmitSetup.delays = kk;
        group.sequence(end+1) = event;
      end

      for kk=2:20
        testcase.verifyEqual(group.sequence(kk).transmitSetup.delays, kk);
      end
    end

    function updateFieldAfterCached(testcase)
      a = urx.Wave();
      testcase.verifyEqual(a.timeZeroReferencePoint.x, 0);
      b = urx.TransmitSetup();
      b.wave = a;
      testcase.verifyEqual(a.timeZeroReferencePoint.x, 0);
      testcase.verifyEqual(b.wave.timeZeroReferencePoint.x, 0);
    end

    function assignVectorSharedInVectorWeak(testcase)
      transmitSetup = urx.TransmitSetup();
      excitation = urx.Excitation();
      excitation.transmitFrequency = 150.;
      transmitSetup.excitations = repmat(excitation, [1, 10]);

      Events = urx.Event();
      Events.transmitSetup = transmitSetup;

      Group = urx.Group();
      Group.sequence = Events;

      Acquisition = urx.Acquisition();
      Acquisition.excitations = [excitation];
      Acquisition.groups = [Group];

      dataset = urx.Dataset();
      dataset.acquisition = Acquisition;
      urx.saveToFile('vector_weak.urx', dataset);

      delete 'vector_weak.urx'
    end

    function testOutput(testcase)
      libBinding = urx.LibBinding.getInstance();

      libBinding.call('urx_cout', ['Hello cout' newline]);
      libBinding.call('urx_cerr', ['Hello cerr' newline]);
      % Really throw and stop the script at the current line.
      % libBinding.call('urx_throw', ['Hello throw' newline]);
    end
  end
end
