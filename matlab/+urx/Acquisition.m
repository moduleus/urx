classdef Acquisition < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    localTime char
    countryCode char
    system char
    
    soundSpeed(1,1) double
    timestamp(1,1) double
    
    probes urx.StdVector
    %excitations(:,1) = urx.Excitation.empty(0,1)
    %waves(:,1) = urx.Wave.empty(0,1)
    %groups(:,1) = urx.Group.empty(0,1)
    %groups_data(:,1) = urx.GroupData.empty(0,1)
  end

  methods
    function this = Acquisition(varargin)
      this@urx.Object(varargin{:});
      this.probes = urx.StdVector('urx.Probe', 1, urx.StdVectorType.SHARED);
    end
  end
end
