classdef RawData < urx.Object
  properties (Access = public, GetObservable)
    size(1,1) int64
  end

  methods
    function this = RawData(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
