classdef Transform < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    rotation urx.Vector3D {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    translation urx.Vector3D {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Transform(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
