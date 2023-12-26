classdef Event < urx.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    transmitSetup urx.TransmitSetup {mustBeScalarOrEmpty}
    receiveSetup urx.ReceiveSetup {mustBeScalarOrEmpty}
  end
end
