classdef RawData < urx.Object
  properties (Access = public, GetObservable)
    size(1,1) int64
  end
  properties (Access = public, GetObservable, SetObservable)
    data
  end

  methods
    function this = RawData(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
