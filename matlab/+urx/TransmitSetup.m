classdef TransmitSetup < urx.Object
  properties (Access = public)
    activeElementsStd
    excitationsStd
    delaysStd
  end
  properties (Access = public, SetObservable, GetObservable)
    probe urx.Probe {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    wave urx.Wave {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    activeElements(1,:) cell

    excitations(1,:) urx.Excitation {urx.Validator.weakPtrInCpp}

    delays(1,:) double

    probeTransform urx.Transform {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    timeOffset(1,1) double
  end

  methods
    function this = TransmitSetup(varargin)
      this@urx.Object(varargin{:});
      this.activeElementsStd = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
    end
  end
end
