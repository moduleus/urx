classdef Transform < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    rotation(1,1) urx.Vector3D
    translation(1,1) urx.Vector3D
  end
end
