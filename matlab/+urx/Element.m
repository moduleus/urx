classdef Element < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    transform urx.Transform {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    
    elementGeometry urx.ElementGeometry {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    impulseResponse urx.ImpulseResponse {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = Element(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
