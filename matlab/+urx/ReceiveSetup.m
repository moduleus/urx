classdef ReceiveSetup < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    samplingFrequency(1,1) double
  end
end
