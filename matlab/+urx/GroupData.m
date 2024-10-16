classdef GroupData < urx.Object
  properties (Access = ?urx.Object)
    sequenceTimestampsStd
    eventTimestampsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    group urx.Group {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    rawData urx.RawData {mustBeScalarOrEmpty, urx.Validator.sharedPtrInCpp}

    groupTimestamp(1,1) double

    sequenceTimestamps(1,:) double
    eventTimestamps(1,:) cell
  end

  methods
    function this = GroupData(varargin)
      this@urx.Object(varargin{:});
      this.eventTimestampsStd = urx.StdVector('double', 2, urx.PtrType.RAW, this);
    end
  end
end
