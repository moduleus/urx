classdef GroupData < urx.Object
  properties (Access = public)
    stdVectorSequenceTimestamps urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    stdVectorEventTimestamps urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    group_timestamp(1,1) double
    sequence_timestamps(:,1) double = []
    event_timestamps(:,1) cell = {double([])}
    % raw_data
    % size_of_data_type
    % data_type
    group urx.Group {mustBeScalarOrEmpty} = urx.Group.empty(0,1)
  end
end
