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
      this.rawData = urx.RawData_double_complex(libpointer, true, 0);
      this.sequenceTimestamps = urx.StdVector('double', 1, urx.StdVectorType.RAW, this);
      this.eventTimestamps = urx.StdVector('double', 2, urx.StdVectorType.RAW, this);
    end
  end
end
