classdef RawData_int16_t_complex < urx.RawData
  methods
    function this = RawData_int16_t_complex(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
