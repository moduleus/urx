classdef ElementGeometry < urx.Object
  properties (Access = public)
    stdVectorVector3D urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    perimeter(:,1) = urx.Vector3D.empty(0,1)
  end
end
