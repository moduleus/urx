classdef Probe < urx.Object
  properties (Access = ?urx.Object)
    elementGeometriesStd
    impulseResponsesStd
    elementsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    description char

    type(1,1) int32

    transform urx.Transform {mustBeScalarOrEmpty}

    elementGeometries(1,:) urx.ElementGeometry
    impulseResponses(1,:) urx.ImpulseResponse

    elements(1,:) urx.Element
  end

  methods
    function this = Probe(varargin)
      this@urx.Object(varargin{:});
      this.elementGeometriesStd = urx.StdVector('urx.ElementGeometry', 1, urx.PtrType.SHARED, this);
      this.impulseResponsesStd = urx.StdVector('urx.ImpulseResponse', 1, urx.PtrType.SHARED, this);

      this.elementsStd = urx.StdVector('urx.Element', 1, urx.PtrType.RAW, this);
    end
  end
end
