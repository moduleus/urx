classdef LibBinding < handle
  properties
    notfound
    warnings
    libName
  end
  
  methods (Access = protected)
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
        if isa(varargin{i},'string')
          varargin{i} = char(varargin{i});
        end
      end
      varargout{:} = calllib(this.libName, varargin{:});
    end

    % May be useful to ease debug.
    function res = showPtr(this, ptr)
      res = uint64(this.call('get_pointer', ptr));
    end
  end
  
  methods (Static)
    function this = getInstance(libraryPath, headerPath, includepaths, envpaths)
      persistent instance
      if isempty(instance)
        setenv("HDF5_DISABLE_VERSION_CHECK", "2");
        if nargin < 4
          scriptPath = fileparts(mfilename('fullpath'));
          % Default path for toolbox
          if ispc()
            instance = urx.LibBinding([scriptPath '/../UrxMatlabBinding.dll'], ...
              [scriptPath '/../include/urx/matlab/bindings.h'], ...
              {[scriptPath '/../include']}, {});
          else
            instance = urx.LibBinding([scriptPath '/../libUrxMatlabBinding.so'], ...
              [scriptPath '/../include/urx/matlab/bindings.h'], ...
              {[scriptPath '/../include']}, {});
          end
        else
          instance = urx.LibBinding(libraryPath, headerPath, includepaths, envpaths);
        end
      end
      this = instance;
    end
  end
end
