classdef RawData_double_complex < urx.RawData
  methods
    function this = RawData_double_complex(varargin)
      this@urx.RawData(libpointer, [], [], varargin{:});
    end
  end
end
