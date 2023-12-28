classdef ImpulseResponse < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    samplingFrequency(1,1) double
    timeOffset(1,1) double

    units char

    data urx.StdVector
  end

  methods
    function this = ImpulseResponse(varargin)
      this@urx.Object(varargin{:});
      this.data = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
