classdef RawData_int32_t_real < urx.RawData
  methods
    function this = RawData_int32_t_real(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
