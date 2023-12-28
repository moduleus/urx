classdef Element < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    transform urx.Transform {mustBeScalarOrEmpty}
    
    elementGeometry urx.ElementGeometry {mustBeScalarOrEmpty}
    impulseResponse urx.ImpulseResponse {mustBeScalarOrEmpty}
  end

  methods
    function this = Element(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
