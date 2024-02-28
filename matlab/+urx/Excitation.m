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
    end
  end
end
