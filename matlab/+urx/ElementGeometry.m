classdef ElementGeometry < urx.Object
  properties (Access = ?urx.Object)
    perimeterStd
  end
  properties (Access = public, SetObservable, GetObservable)
    perimeter(1,:) urx.Vector3D {urx.Validator.rawInCpp}
  end

  methods
    function this = ElementGeometry(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
