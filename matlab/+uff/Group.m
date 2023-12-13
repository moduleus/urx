classdef Group < uff.Object
  properties (Access = public)
    stdVectorSequence uff.StdVector {mustBeScalarOrEmpty} = uff.StdVector.empty(0,1)
  end

  properties (Access = public, SetObservable, GetObservable)
    sampling_type(1,1) uff.Group.SamplingType
    sequence(:,1) = uff.Event.empty(0,1)
    description char
  end
end
