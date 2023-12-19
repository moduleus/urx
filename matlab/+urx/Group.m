classdef Group < urx.Object
  properties (Access = public)
    stdVectorSequence urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    sampling_type(1,1) urx.Group.SamplingType
    sequence(:,1) = urx.Event.empty(0,1)
    description char
  end
end
