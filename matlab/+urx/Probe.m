classdef Probe < urx.Object
  properties (Access = public)
    stdVectorElementGeometry urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    stdVectorImpulseResponse urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    stdVectorElement urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    description char

    type(1,1) urx.Probe.ProbeType

    transform(1,1) urx.Transform
  
    elementGeometries(:,1) = urx.ElementGeometry.empty(0,1)
    impulseResponses(:,1) = urx.ImpulseResponse.empty(0,1)

    elements(:,1) = urx.Element.empty(0,1)
  end
end
