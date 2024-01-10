classdef GroupData < urx.Object
  properties (Access = public)
    sequenceTimestampsStd
    eventTimestampsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    group urx.Group {mustBeScalarOrEmpty}
    rawData urx.RawData {mustBeScalarOrEmpty}

    groupTimestamp(1,1) double

    sequenceTimestamps(1,:) double
    eventTimestamps(1,:) cell
  end

  methods
    function this = GroupData(varargin)
      this@urx.Object(varargin{:});
      this.sequenceTimestampsStd = urx.StdVector('double', 1, urx.PtrType.RAW, this);
      this.eventTimestampsStd = urx.StdVector('double', 2, urx.PtrType.RAW, this);
    end
  end
end
