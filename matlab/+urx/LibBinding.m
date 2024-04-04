classdef LibBinding < handle
  properties
    notfound
    warnings
    libName
  end
  
  methods (Access = private)
    function this = LibBinding(libraryPath, headerPath, includepaths, envpaths)
      splitPath = split(libraryPath, ["\" "/"]);
      splitName = split(splitPath(end), ".");
      finaleName = join(splitName(1:end-1), ".");
      this.libName = finaleName{1};

      args = {libraryPath, headerPath};

      for i = numel(includepaths):-1:1
        args{1+i*2+1} = includepaths{i};
        args{1+i*2} = 'includepath';
      end 

      for i = 1:numel(envpaths)
        addpath(envpaths{i});
      end 

      [this.notfound, this.warnings] = loadlibrary(args{:});
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
    function this = getInstance(libraryPath, headerPath, includepaths, envpaths)
      persistent instance
      if isempty(instance)
        if nargin < 3
          instance = urx.LibBinding();
        else
          instance = urx.LibBinding(libraryPath, headerPath, includepaths, envpaths);
        end
      end
      this = instance;
    end
  end
end
