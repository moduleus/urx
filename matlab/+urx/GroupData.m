classdef GroupData < urx.Object
  properties (Access = public)
    stdVectorSequenceTimestamps urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
    stdVectorEventTimestamps urx.StdVector {mustBeScalarOrEmpty} = urx.StdVector.empty(0,1)
  end
  
  properties (Access = public, SetObservable, GetObservable)
    groupTimestamp(1,1) double
    sequenceTimestamps(:,1) double = []
    eventTimestamps(:,1) cell = {double([])}
    % raw_data
    % size_of_data_type
    % data_type
    group urx.Group {mustBeScalarOrEmpty} = urx.Group.empty(0,1)
  end
end
