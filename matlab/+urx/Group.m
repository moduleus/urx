classdef Group < urx.Object
  properties (Access = ?urx.Object)
    sequenceStd
  end
  properties (Access = public, SetObservable, GetObservable)
    samplingType(1,1) int32
    dataType(1,1) int32

    description char

    soundSpeed(1,1) double

    sequence(1,:) urx.Event {urx.Validator.rawInCpp}
  end

  methods
    function this = Group(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
