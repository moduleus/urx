classdef Probe < urx.Object
  properties (Access = ?urx.Object)
    elementGeometriesStd
    impulseResponsesStd
    elementsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    description char

    type(1,1) int32

    transform urx.Transform {mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    elementGeometries(1,:) urx.ElementGeometry {urx.Validator.sharedPtrInCpp}
    impulseResponses(1,:) urx.ImpulseResponse {urx.Validator.sharedPtrInCpp}

    elements(1,:) urx.Element {urx.Validator.rawInCpp}
  end

  methods
    function this = Probe(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
