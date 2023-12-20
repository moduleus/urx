classdef Element < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    transform urx.Transform {mustBeScalarOrEmpty}
    
    elementGeometry urx.ElementGeometry {mustBeScalarOrEmpty}
    impulseResponse urx.ImpulseResponse {mustBeScalarOrEmpty}
  end
end
