classdef ReceiveSetup < urx.Object
  properties (Access = ?urx.Object)
    activeElementsStd
    tgcProfileStd
  end
  properties (Access = public, SetObservable, GetObservable)
    probe urx.Probe {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}

    probeTransform urx.Transform {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    samplingFrequency(1,1) double

    numberSamples(1,1) uint32

    activeElements(1,:) cell

    tgcProfile(1,:) double
    tgcSamplingFrequency(1,1) double

    modulationFrequency(1,1) double
    timeOffset(1,1) double
  end

  methods
    function this = ReceiveSetup(varargin)
      this@urx.Object(varargin{:});
      this.activeElementsStd = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
    end
  end
end
