classdef Acquisition < urx.Object
  properties (Access = ?urx.Object)
    probesStd
    excitationsStd
    groupsStd
    groupsDataStd
  end
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    localTime char
    countryCode char
    system char
    
    timestamp(1,1) double
    
    probes(1,:) urx.Probe {urx.Validator.sharedPtrInCpp}
    excitations(1,:) urx.Excitation {urx.Validator.sharedPtrInCpp}
    groups(1,:) urx.Group {urx.Validator.sharedPtrInCpp}
    groupsData(1,:) urx.GroupData {urx.Validator.rawInCpp}
  end

  methods
    function this = Acquisition(varargin)
      this@urx.Object(varargin{:});
    end
  end
end
