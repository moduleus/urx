classdef LibBinding < handle
  properties
    notfound
    warnings
  end

  methods (Access = private)
    function this = LibBinding()
      currentPath = [fileparts(mfilename('fullpath')) '/'];
      [this.notfound, this.warnings] = loadlibrary(...
          [currentPath '../../build/matlab/LibBinding/libUrxLibBinding.so'], ...
          [currentPath '../LibBinding/UrxLibBinding.h']);
    end

    function delete(this)
      unloadlibrary libUrxLibBinding
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

    function varargout = call(varargin)
      varargout{:} = calllib('libUrxLibBinding', varargin{:});
    end
  end
end
