classdef RawData_int32_t_complex < urx.RawData
  methods
    function this = RawData_int32_t_complex(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
