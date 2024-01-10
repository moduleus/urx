classdef RawData_float_real < urx.RawData
  methods
    function this = RawData_float_real(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
