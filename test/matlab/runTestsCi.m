function runTestsCi
  addpath([pwd '/../../matlab']);
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
  lastwarn('');
  results = run(suite);
  
  for i=1:numel(results)
    assert( results(i).Failed == 0, 'Unit tests failed' )
    assert( results(i).Incomplete == 0, 'Unit tests failed' )
  end

  assert(isempty(lastwarn));
end