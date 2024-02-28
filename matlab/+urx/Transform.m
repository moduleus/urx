classdef Transform < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    rotation urx.Vector3D {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    translation urx.Vector3D {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end
end
