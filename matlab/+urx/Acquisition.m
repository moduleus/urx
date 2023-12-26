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
    excitations urx.StdVector
    waves urx.StdVector
    groups urx.StdVector
    %groups_data(:,1) = urx.GroupData.empty(0,1)
  end

  methods
    function this = Acquisition(varargin)
      this@urx.Object(varargin{:});
      this.probes = urx.StdVector('urx.Probe', 1, urx.StdVectorType.SHARED, this);
      this.excitations = urx.StdVector('urx.Excitation', 1, urx.StdVectorType.SHARED, this);
      this.waves = urx.StdVector('urx.Wave', 1, urx.StdVectorType.SHARED, this);
      this.groups = urx.StdVector('urx.Group', 1, urx.StdVectorType.SHARED, this);
    end
  end
end
