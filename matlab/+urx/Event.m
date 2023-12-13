classdef Event < urx.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    % transmit_setup(1,1) urx.TransmitSetup
    receive_setup(1,1) urx.ReceiveSetup
  end
end
