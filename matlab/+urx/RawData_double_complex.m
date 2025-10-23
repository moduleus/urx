classdef RawData_double_complex < urx.RawData
  methods
    function this = RawData_double_complex(varargin)
      if nargin == 0 || ~isa(varargin{1}, 'urx.PtrType')
        varargin = [{[]}, varargin(:)'];
      end
      this@urx.RawData(varargin{:});
    end
  end
end
