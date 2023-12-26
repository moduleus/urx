classdef ElementGeometry < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    perimeter urx.StdVector
  end

  methods
    function this = ElementGeometry(varargin)
      this@urx.Object(varargin{:});
      this.perimeter = urx.StdVector('urx.Vector3D', 1, urx.StdVectorType.RAW, this);
    end
  end
end
