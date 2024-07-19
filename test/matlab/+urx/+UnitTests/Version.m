classdef Version < matlab.unittest.TestCase
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
    function testVersion(testcase)
      libBinding = urx.LibBinding.getInstance();

      version = urx.Version();

      % Check that only needed properties are visible
      testcase.verifyEqual(numel(properties(version)), 3);

      % Modify object directly
      version.major = 852;
      version.minor = 111;
      version.patch = 963;

      % Check value. Should read data from C++.
      testcase.verifyEqual(version.major, uint16(852));
      testcase.verifyEqual(version.minor, uint16(111));
      testcase.verifyEqual(version.patch, uint16(963));

      % Modify data without using urx.Object.
      ptrMajor = libBinding.call('urx_Version_shared_major', version);
      ptrMajor.setdatatype('uint16Ptr', 1, 1);
      ptrMinor = libBinding.call('urx_Version_shared_minor', version);
      ptrMinor.setdatatype('uint16Ptr', 1, 1);
      ptrPatch = libBinding.call('urx_Version_shared_patch', version);
      ptrPatch.setdatatype('uint16Ptr', 1, 1);

      % Check that pointer is valid.
      testcase.verifyEqual(ptrMajor.Value, uint16(852));
      testcase.verifyEqual(ptrMinor.Value, uint16(111));
      testcase.verifyEqual(ptrPatch.Value, uint16(963));

      % Modify value by using the pointer.
      ptrMajor.Value = 777;
      ptrMinor.Value = 666;
      ptrPatch.Value = 555;

      % Check that version really read data from C++.
      testcase.verifyEqual(version.major, uint16(777));
      testcase.verifyEqual(version.minor, uint16(666));
      testcase.verifyEqual(version.patch, uint16(555));
    end

    function testParent(testcase)
      libBinding = urx.LibBinding.getInstance();

      dataset = urx.Dataset();

      % Modify version when version is embedded in another structure.
      dataset.version.major = 852;
      dataset.version.minor = 111;
      dataset.version.patch = 963;

      testcase.verifyEqual(dataset.version.major, uint16(852));
      testcase.verifyEqual(dataset.version.minor, uint16(111));
      testcase.verifyEqual(dataset.version.patch, uint16(963));

      % Modify data without using urx.Object.
      ptrMajor = libBinding.call('urx_Version_raw_major', dataset.version);
      ptrMajor.setdatatype('uint16Ptr', 1, 1);
      ptrMinor = libBinding.call('urx_Version_raw_minor', dataset.version);
      ptrMinor.setdatatype('uint16Ptr', 1, 1);
      ptrPatch = libBinding.call('urx_Version_raw_patch', dataset.version);
      ptrPatch.setdatatype('uint16Ptr', 1, 1);

      % Check that pointer is valid.
      testcase.verifyEqual(ptrMajor.Value, uint16(852));
      testcase.verifyEqual(ptrMinor.Value, uint16(111));
      testcase.verifyEqual(ptrPatch.Value, uint16(963));

      % Modify value by using the pointer.
      ptrMajor.Value = 777;
      ptrMinor.Value = 666;
      ptrPatch.Value = 555;

      % Check that version really read data from C++.
      testcase.verifyEqual(dataset.version.major, uint16(777));
      testcase.verifyEqual(dataset.version.minor, uint16(666));
      testcase.verifyEqual(dataset.version.patch, uint16(555));

      % Now check that I can assign a version object in a parent object.
      version = urx.Version();

      version.major = 1;
      version.minor = 2;
      version.patch = 3;

      dataset.version = version;

      % Check that both version are good.
      testcase.verifyEqual(version.major, uint16(1));
      testcase.verifyEqual(version.minor, uint16(2));
      testcase.verifyEqual(version.patch, uint16(3));
      testcase.verifyEqual(dataset.version.major, uint16(1));
      testcase.verifyEqual(dataset.version.minor, uint16(2));
      testcase.verifyEqual(dataset.version.patch, uint16(3));

      % Update version
      version.major = 4;
      version.minor = 5;
      version.patch = 6;

      testcase.verifyEqual(version.major, uint16(4));
      testcase.verifyEqual(version.minor, uint16(5));
      testcase.verifyEqual(version.patch, uint16(6));
      testcase.verifyEqual(dataset.version.major, uint16(4));
      testcase.verifyEqual(dataset.version.minor, uint16(5));
      testcase.verifyEqual(dataset.version.patch, uint16(6));
      
      % Update data.version
      dataset.version.major = 7;
      dataset.version.minor = 8;
      dataset.version.patch = 9;

      testcase.verifyEqual(version.major, uint16(7));
      testcase.verifyEqual(version.minor, uint16(8));
      testcase.verifyEqual(version.patch, uint16(9));
      testcase.verifyEqual(dataset.version.major, uint16(7));
      testcase.verifyEqual(dataset.version.minor, uint16(8));
      testcase.verifyEqual(dataset.version.patch, uint16(9));

      clear dataset
      % dataset is cleared. But still alive in version.parent.

      version.major = 10;
      version.minor = 11;
      version.patch = 12;
    
      testcase.verifyEqual(version.major, uint16(10));
      testcase.verifyEqual(version.minor, uint16(11));
      testcase.verifyEqual(version.patch, uint16(12));
      testcase.verifyEqual(ptrMajor.Value, uint16(10));
      testcase.verifyEqual(ptrMinor.Value, uint16(11));
      testcase.verifyEqual(ptrPatch.Value, uint16(12));
    end
  end
end