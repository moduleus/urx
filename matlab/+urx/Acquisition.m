classdef Acquisition < uff.Object % handle
  properties (Access = public)
    stdVectorGroups uff.StdVector {mustBeScalarOrEmpty} = uff.StdVector.empty(0,1)
    stdVectorUniqueReceiveSetups uff.StdVector {mustBeScalarOrEmpty} = uff.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    local_time char
    country_code char
    system char
    sound_speed(1,1) double
    groups(:,1) = uff.Group.empty(0,1)
    unique_receive_setups(:,1) = uff.ReceiveSetup.empty(0,1)
  end
end
