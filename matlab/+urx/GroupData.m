classdef GroupData < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    group urx.Group {mustBeScalarOrEmpty}
    %std::shared_ptr<RawData> raw_data;

    groupTimestamp double

    sequenceTimestamps urx.StdVector
    eventTimestamps urx.StdVector
  end

  methods
    function this = GroupData(varargin)
      this@urx.Object(varargin{:});
      this.sequenceTimestamps = urx.StdVector('double', 1, urx.StdVectorType.RAW, this);
      this.eventTimestamps = urx.StdVector('double', 2, urx.StdVectorType.RAW, this);
    end
  end
end
