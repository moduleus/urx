classdef Group < urx.Object
  properties (Access = public)
    sequenceStd
  end
  properties (Access = public, SetObservable, GetObservable)
    samplingType(1,1) int32
    dataType(1,1) int32

    description char

    sequence(1,:) urx.Event
  end

  methods
    function this = Group(varargin)
      this@urx.Object(varargin{:});
      this.sequenceStd = urx.StdVector('urx.Event', 1, urx.PtrType.RAW, this);
    end
  end
end
