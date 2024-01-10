classdef RawData_int16_t_real < urx.RawData
  methods
    function this = RawData_int16_t_real(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
