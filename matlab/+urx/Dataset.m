classdef Dataset < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    version urx.Version {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    acquisition urx.Acquisition {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Dataset(varargin)
      this@urx.Object(varargin{:});
    end

    function saveToFile(this, filename)
      this.libBindingRef.call('urx_save_to_file', filename, this.id);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      dataset_ptr = urx.LibBinding.getInstance().call('urx_load_from_file', filename);
      this = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end
end
