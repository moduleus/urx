function runTestsCi
  addpath([pwd '/../../matlab']);
  if ispc()
    P = matlab.unittest.parameters.Parameter.fromData(...
      'libraryPath', {[pwd '/../../CI/matlab/LibBinding/Release/UrxLibBinding.dll']},...
      'headerPath', {[pwd '/../../matlab/LibBinding/UrxLibBinding.h']});
  else
    P = matlab.unittest.parameters.Parameter.fromData(...
      'libraryPath', {[pwd '/../../CI/matlab/LibBinding/libUrxLibBinding.so']},...
      'headerPath', {[pwd '/../../matlab/LibBinding/UrxLibBinding.h']});
  end
  suite = matlab.unittest.TestSuite.fromFolder("+urx/+UnitTests", ...
    'ExternalParameters', P);
  results = run(suite);
  
  for i=1:numel(results)
    assert( results(i).Failed == 0, 'Unit tests failed' )
  end
end
