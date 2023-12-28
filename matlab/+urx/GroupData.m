classdef GroupData < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    group urx.Group {mustBeScalarOrEmpty}
    rawData urx.RawData {mustBeScalarOrEmpty}

    groupTimestamp(1,1) double

    sequenceTimestamps urx.StdVector
    eventTimestamps urx.StdVector
  end

  methods
    function this = GroupData(varargin)
      this@urx.Object(varargin{:});
      this.sequenceTimestamps = urx.StdVector('double', 1, urx.PtrType.RAW, this);
      this.eventTimestamps = urx.StdVector('double', 2, urx.PtrType.RAW, this);
    end
  end
end
