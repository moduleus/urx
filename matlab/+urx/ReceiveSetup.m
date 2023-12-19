classdef ReceiveSetup < urx.Object
  properties (Access = public)
    % stdVector... {mustBeScalarOrEmpty}
  end
  
  properties (Access = public, SetObservable, GetObservable)
    sampling_frequency(1,1) double
  end
end
