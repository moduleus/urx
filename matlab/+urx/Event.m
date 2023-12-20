classdef Event < urx.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    % transmit_setup(1,1) urx.TransmitSetup
    receiveSetup urx.ReceiveSetup {mustBeScalarOrEmpty}
  end
end
