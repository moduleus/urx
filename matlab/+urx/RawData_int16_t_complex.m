classdef RawData_int16_t_complex < urx.RawData
  methods
    function this = RawData_int16_t_complex(varargin)
      if nargin == 0 || ~isa(varargin{1}, 'lib.pointer')
        varargin = [{libpointer}, {[]}, {[]}, varargin(:)'];
      end
      this@urx.RawData(varargin{:});
    end
  end
end