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
      warning('urx.Dataset.saveTofile is depreciated. Use static method urx.saveToFile(filename, dataset).');
      this.libBindingRef.call('urx_save_to_file', filename, this.id);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      warning('urx.Dataset.loadFromFile is depreciated. Use static method urx.loadFromFile(filename).');
      libBinding = urx.LibBinding.getInstance();
      dataset_ptr = libBinding.call('urx_load_from_file', filename);
      this = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end
end
