classdef Wave < urx.Object
  properties (Access = public, SetObservable, GetObservable)

    type(1,1) int32

    timeZero(1,1) double

    timeZeroReferencePoint urx.Vector3D {mustBeScalarOrEmpty}

    channelMapping urx.StdVector

    channelExcitations urx.StdVector

    channelDelays urx.StdVector

    parameters urx.StdVector
  end

  methods
    function this = Wave(varargin)
      this@urx.Object(varargin{:});
      this.channelMapping = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
      this.channelExcitations = urx.StdVector('urx.Excitation', 1, urx.PtrType.WEAK, this);
      this.channelDelays = urx.StdVector('double', 1, urx.PtrType.RAW, this);
      this.parameters = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
