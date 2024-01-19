classdef RawData_float_complex < urx.RawData
  methods
    function this = RawData_float_complex(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
