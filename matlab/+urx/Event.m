classdef Event < urx.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    transmitSetup urx.TransmitSetup {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    receiveSetup urx.ReceiveSetup {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Event(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
