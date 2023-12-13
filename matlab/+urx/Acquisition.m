classdef Acquisition < urx.Object % handle
  properties (Access = public)
    stdVectorGroups urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    stdVectorUniqueReceiveSetups urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    local_time char
    country_code char
    system char
    sound_speed(1,1) double
    groups(:,1) = urx.Group.empty(0,1)
    unique_receive_setups(:,1) = urx.ReceiveSetup.empty(0,1)
  end
end
