classdef Wave < urx.Object
  properties (Access = public)
    parametersStd
  end
  properties (Access = public, SetObservable, GetObservable)

    type(1,1) int32

    timeZero(1,1) double

    timeZeroReferencePoint urx.Vector3D {mustBeScalarOrEmpty}

    parameters(1,:) double
  end

  methods
    function this = Wave(varargin)
      this@urx.Object(varargin{:});
      this.parametersStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
