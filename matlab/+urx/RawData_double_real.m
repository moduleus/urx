classdef RawData_double_real < urx.RawData
  methods
    function this = RawData_double_real(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
