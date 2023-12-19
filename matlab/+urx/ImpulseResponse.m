classdef ImpulseResponse < urx.Object
  properties (Access = public)
    stdVectorVector3D urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    sampling_frequency(1,1) double
    time_offset(1,1) double
    
    data(:,1) double
  end
end
