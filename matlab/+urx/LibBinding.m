classdef LibBinding < handle
  properties
    notfound
    warnings
  end

  methods (Access = private)
    function this = LibBinding()
      currentPath = [fileparts(mfilename('fullpath')) '/'];
      [this.notfound, this.warnings] = loadlibrary(...
          [currentPath '../../build/matlab/LibBinding/libUffLibBinding.so'], ...
          [currentPath '../LibBinding/UffLibBinding.h']);
    end

    function delete(this)
      unloadlibrary libUffLibBinding
    end
  end

  methods (Static)
    function this = getInstance()
      persistent instance
      if isempty(instance)
        instance = uff.LibBinding();
      end
        this = instance;
    end

    function varargout = call(varargin)
      varargout{:} = calllib('libUffLibBinding', varargin{:});
    end
  end
end
