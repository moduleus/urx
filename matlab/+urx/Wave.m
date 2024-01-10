classdef Wave < urx.Object
  properties (Access = public)
    channelMappingStd
    channelExcitationsStd
    channelDelaysStd
    parametersStd
  end
  properties (Access = public, SetObservable, GetObservable)

    type(1,1) int32

    timeZero(1,1) double

    timeZeroReferencePoint urx.Vector3D {mustBeScalarOrEmpty}

    channelMapping(1,:) cell

    channelExcitations(1,:) urx.Excitation

    channelDelays(1,:) double

    parameters(1,:) double
  end

  methods
    function this = Wave(varargin)
      this@urx.Object(varargin{:});
      this.channelMappingStd = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
      this.channelExcitationsStd = urx.StdVector('urx.Excitation', 1, urx.PtrType.WEAK, this);
      this.channelDelaysStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
      this.parametersStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
