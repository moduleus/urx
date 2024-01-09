classdef Transform < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    rotation urx.Vector3D {mustBeScalarOrEmpty}
    translation urx.Vector3D {mustBeScalarOrEmpty}
  end
end
