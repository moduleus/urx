classdef Acquisition < urx.Object
  properties (Access = public)
    %stdVectorProbes urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    %stdVectorExcitations urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    %stdVectorWaves urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    %stdVectorGroups urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    %stdVectorGroupsData urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    local_time char
    country_code char
    system char
    
    sound_speed(1,1) double
    timestamp(1,1) double
    
    %probes(:,1) = urx.Probe.empty(0,1)
    %excitations(:,1) = urx.Excitation.empty(0,1)
    %waves(:,1) = urx.Wave.empty(0,1)
    %groups(:,1) = urx.Group.empty(0,1)
    %groups_data(:,1) = urx.GroupData.empty(0,1)
  end
end
