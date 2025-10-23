classdef RawData_int32_t_real < urx.RawData
  methods
    function this = RawData_int32_t_real(varargin)
      if nargin == 0 || ~isa(varargin{1}, 'urx.PtrType')
        varargin = [{[]}, varargin(:)'];
      end
      this@urx.RawData(varargin{:});
    end
  end
end
