classdef LibBinding < handle
  properties
    notfound
    warnings
    libName
  end
  
  methods (Access = private)
    function this = LibBinding(libraryPath, headerPath)
      currentPath = [fileparts(mfilename('fullpath')) '/'];
      if nargin >= 1
        splitPath = split(libraryPath, ["\" "/"]);
        splitName = split(splitPath(end), ".");
        finaleName = join(splitName(1:end-1), ".");
        this.libName = finaleName{1};
      else
        if isunix()
          this.libName = 'libUrxLibBinding';
          libraryPath = [currentPath '../../../urx-build/matlab/LibBinding/' this.libName '.so'];
        else
            this.libName = 'UrxLibBinding';
            libraryPath = [currentPath '../../../urx_build/Matlab/LibBinding/Release/' this.libName '.dll'];
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
      for i = 1:numel(varargin)
        if isa(varargin{i}, 'urx.ObjectField')
          varargin{i} = varargin{i}.id;
        end
      end
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
