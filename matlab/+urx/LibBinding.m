classdef LibBinding < handle
  properties
    notfound
    warnings
    libName
  end
  
  methods (Access = private)
    function this = LibBinding(libraryPath, headerPath)
      currentPath = [fileparts(mfilename('fullpath')) '/'];
      if isunix()
        this.libName = 'libUrxLibBinding';
        if nargin < 1
          libraryPath = [currentPath '../../../urx-build/matlab/LibBinding/' this.libName '.so'];
        end
      else
        this.libName = 'UrxLibBinding';
        if nargin < 1
          libraryPath = [currentPath '../../../urx_build/Matlab/LibBinding/Debug/' this.libName '.dll'];
        end
      end
      if nargin < 2 || isempty(headerPath)
        headerPath = [currentPath '../LibBinding/UrxLibBinding.h'];
      end
      [this.notfound, this.warnings] = loadlibrary(libraryPath, headerPath);
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
    function this = getInstance(libraryPath, headerPath)
      persistent instance
      if isempty(instance)
        if nargin < 2
          instance = urx.LibBinding();
        else
          instance = urx.LibBinding(libraryPath, headerPath);
        end
      end
      this = instance;
    end
  end
end
