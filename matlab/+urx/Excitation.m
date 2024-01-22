classdef Excitation < urx.Object
  properties (Access = ?urx.Object)
    waveformStd
  end
  properties (Access = public, SetObservable, GetObservable)
    pulseShape char

    transmitFrequency(1,1) double
    samplingFrequency(1,1) double

    waveform(1,:) double
  end

  methods
    function this = Excitation(varargin)
      this@urx.Object(varargin{:});
      this.waveformStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
    end
  end
end
