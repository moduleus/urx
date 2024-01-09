classdef TransmitSetup < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    probe urx.Probe {mustBeScalarOrEmpty}
    wave urx.Wave {mustBeScalarOrEmpty}

    probeTransform urx.Transform {mustBeScalarOrEmpty}

    timeOffset(1,1) double
  end

  methods
    function this = TransmitSetup(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
