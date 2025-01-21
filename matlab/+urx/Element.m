classdef Element < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    transform urx.Transform {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    
    elementGeometry urx.ElementGeometry {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    impulseResponse urx.ImpulseResponse {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = Element(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
