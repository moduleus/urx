classdef LibBinding < handle
  properties
    notfound
    warnings
    libName
  end

  methods (Access = private)
    function this = LibBinding()
      currentPath = [fileparts(mfilename('fullpath')) '/'];
      if isunix()
          this.libName = 'libUrxLibBinding';
          [this.notfound, this.warnings] = loadlibrary(...
              [currentPath '../../build/matlab/LibBinding/libUrxLibBinding.so'], ...
              [currentPath '../LibBinding/UrxLibBinding.h']);
      else
          this.libName = 'UrxLibBinding';
          [this.notfound, this.warnings] = loadlibrary(...
              [currentPath '../../../urx_build/Matlab/LibBinding/Release/UrxLibBinding.dll'], ...
              [currentPath '../LibBinding/UrxLibBinding.h']);
      end
    end

    function delete(this)
        unloadlibrary(this.libName);
    end
  end

  methods
    function varargout = call(this, varargin)
      varargout{:} = calllib(this.libName, varargin{:});
    end
  end
  
  methods (Static)
    function this = getInstance()
      persistent instance
      if isempty(instance)
        instance = urx.LibBinding();
      end
        this = instance;
    end
  end
end
