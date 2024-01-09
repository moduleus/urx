classdef ReceiveSetup < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    probe urx.Probe {mustBeScalarOrEmpty}

    probeTransform urx.Transform {mustBeScalarOrEmpty}

    samplingFrequency(1,1) double

    numberSamples(1,1) uint32

    channelMapping urx.StdVector

    tgcProfile urx.StdVector
    tgcSamplingFrequency(1,1) double

    modulationFrequency(1,1) double
    timeOffset(1,1) double
  end

  methods
    function this = ReceiveSetup(varargin)
      this@urx.Object(varargin{:});
      this.channelMapping = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
      this.tgcProfile = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
