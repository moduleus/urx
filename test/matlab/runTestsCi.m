function runTestsCi
  addpath([pwd '/../../matlab']);
  setenv("HDF5_DISABLE_VERSION_CHECK", "1");
  if ispc()
    P = matlab.unittest.parameters.Parameter.fromData(...
      'libraryPath', {[pwd '/../../CI/matlab/binding/Release/UrxMatlabBinding.dll']},...
      'headerPath', {[pwd '/../../matlab/binding/urx/matlab/bindings.h']},...
      'includepaths', {{[pwd '/../../matlab/binding'], [pwd '/../../CI/matlab/binding'], [pwd '/../../CI']}},...
      'envpaths', {{}});
  else
    P = matlab.unittest.parameters.Parameter.fromData(...
      'libraryPath', {[pwd '/../../CI/matlab/binding/libUrxMatlabBinding.so']},...
      'headerPath', {[pwd '/../../matlab/binding/urx/matlab/bindings.h']},...
      'includepaths', {{[pwd '/../../matlab/binding'], [pwd '/../../CI/matlab/binding'], [pwd '/../../CI']}},...
      'envpaths', {{}});
  end
  suite = matlab.unittest.TestSuite.fromFolder("+urx/+UnitTests", ...
    'ExternalParameters', P);
  results = run(suite);
  
  for i=1:numel(results)
    assert( results(i).Failed == 0, 'Unit tests failed' )
  end
end
