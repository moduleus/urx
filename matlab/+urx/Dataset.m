classdef Dataset < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    version urx.Version {mustBeScalarOrEmpty}
    acquisition urx.Acquisition {mustBeScalarOrEmpty}
  end

  methods
    function this = Dataset(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
