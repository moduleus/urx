classdef Probe < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    description char

    type(1,1) int32

    transform urx.Transform {mustBeScalarOrEmpty}

    elementGeometries urx.StdVector
    impulseResponses urx.StdVector

    elements urx.StdVector
  end

  methods
    function this = Probe(varargin)
      this@urx.Object(varargin{:});
      this.elementGeometries = urx.StdVector('urx.ElementGeometry', 1, urx.PtrType.SHARED, this);
      this.impulseResponses = urx.StdVector('urx.ImpulseResponse', 1, urx.PtrType.SHARED, this);

      this.elements = urx.StdVector('urx.Element', 1, urx.PtrType.RAW, this);
    end
  end
end
