classdef Dataset < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    version urx.Version {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    acquisition urx.Acquisition {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
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

      this.libBindingRef.call('save_to_file', filename, this.id);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      arguments
        filename char
      end

      this = urx.LibBinding.getInstance().call('load_from_file', filename);
    end
  end
end
