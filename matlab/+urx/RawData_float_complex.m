classdef RawData_float_complex < urx.RawData
  methods
    function this = RawData_float_complex(varargin)
      if nargin == 0 || ~isa(varargin{1}, 'lib.pointer')
        varargin = [{libpointer}, {[]}, {[]}, varargin(:)'];
      end
      this@urx.RawData(varargin{:});
    end
  end
end
