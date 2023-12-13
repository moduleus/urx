classdef Event < uff.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    % transmit_setup(1,1) uff.TransmitSetup
    receive_setup(1,1) uff.ReceiveSetup
  end
end
