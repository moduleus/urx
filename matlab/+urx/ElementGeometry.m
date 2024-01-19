classdef ElementGeometry < urx.Object
  properties (Access = public)
    perimeterStd
  end
  properties (Access = public, SetObservable, GetObservable)
    perimeter(1,:) urx.Vector3D
  end

  methods
    function this = ElementGeometry(varargin)
      this@urx.Object(varargin{:});
      this.perimeterStd = urx.StdVector('urx.Vector3D', 1, urx.PtrType.RAW, this);
    end
  end
end
