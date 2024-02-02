classdef Dataset < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    version urx.Version {mustBeScalarOrEmpty}
    acquisition urx.Acquisition {mustBeScalarOrEmpty}
  end

  methods
    function this = Dataset(varargin)
      this@urx.Object(varargin{:});
    end

    function saveToFile(this, filename)
      arguments
        this
        filename char
      end

      this.libBindingRef.call('saveToFile', filename, this.id);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      arguments
        filename char
      end

      this = urx.LibBinding.getInstance().call('loadFromFile', filename);
    end
  end
end
