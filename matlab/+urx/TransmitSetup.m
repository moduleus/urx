classdef TransmitSetup < urx.Object
  properties (Access = public)
    activeElementsStd
    excitationsStd
    delaysStd
  end
  properties (Access = public, SetObservable, GetObservable)
    probe urx.Probe {mustBeScalarOrEmpty}
    wave urx.Wave {mustBeScalarOrEmpty}

    activeElements(1,:) cell

    excitations(1,:) urx.Excitation

    delays(1,:) double

    probeTransform urx.Transform {mustBeScalarOrEmpty}

    timeOffset(1,1) double
  end

  methods
    function this = TransmitSetup(varargin)
      this@urx.Object(varargin{:});
      this.activeElementsStd = urx.StdVector('uint32', 2, urx.PtrType.RAW, this);
      this.excitationsStd = urx.StdVector('urx.Excitation', 1, urx.PtrType.WEAK, this);
      this.delaysStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
