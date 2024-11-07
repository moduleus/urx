classdef RawData < urx.Object
  properties (Access = public, GetObservable)
    size(1,1) int64
  end
  properties (Access = public, GetObservable, SetObservable)
    data
  end

  methods
    function this = RawData(varargin)
      this@urx.Object(varargin{:});
    end
  end

  methods (Access = public)
    function res = samplingType(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') urx.Object.functionPtrType(this.ptrType) '_sampling_type'], this.id);
    end
    function res = dataType(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') urx.Object.functionPtrType(this.ptrType) '_data_type'], this.id);
    end
  end
end
