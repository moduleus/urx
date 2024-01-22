classdef Acquisition < urx.Object
  properties (Access = ?urx.Object)
    probesStd
    excitationsStd
    wavesStd
    groupsStd
    groupsDataStd
  end
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    localTime char
    countryCode char
    system char
    
    soundSpeed(1,1) double
    timestamp(1,1) double
    
    probes(1,:) urx.Probe
    excitations(1,:) urx.Excitation
    waves(1,:) urx.Wave
    groups(1,:) urx.Group
    groupsData(1,:) urx.GroupData
  end

  methods
    function this = Acquisition(varargin)
      this@urx.Object(varargin{:});
      this.probesStd = urx.StdVector('urx.Probe', 1, urx.PtrType.SHARED, this);
      this.excitationsStd = urx.StdVector('urx.Excitation', 1, urx.PtrType.SHARED, this);
      this.wavesStd = urx.StdVector('urx.Wave', 1, urx.PtrType.SHARED, this);
      this.groupsStd = urx.StdVector('urx.Group', 1, urx.PtrType.SHARED, this);
      this.groupsDataStd = urx.StdVector('urx.GroupData', 1, urx.PtrType.RAW, this);
    end
  end
end
