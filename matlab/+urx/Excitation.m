classdef Excitation < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    pulseShape char

    transmitFrequency(1,1) double
    samplingFrequency(1,1) double

    waveform urx.StdVector
  end

  methods
    function this = Excitation(varargin)
      this@urx.Object(varargin{:});
      this.waveform = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
