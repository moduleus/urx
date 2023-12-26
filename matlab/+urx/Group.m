classdef Group < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    samplingType(1,1) int32
    dataType(1,1) int32

    description char

    sequence urx.StdVector
  end

  methods
    function this = Group(varargin)
      this@urx.Object(varargin{:});
      this.sequence = urx.StdVector('urx.Event', 1, urx.StdVectorType.RAW, this);
    end
  end
end
