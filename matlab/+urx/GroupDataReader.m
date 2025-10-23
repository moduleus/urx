classdef GroupDataReader < urx.Object
  methods
    function this = GroupDataReader(group_data)
      idLocal = urx.LibBinding.getInstance().call(['urx_GroupDataReader_new' urx.Object.functionPtrType(urx.PtrType.SHARED) urx.Object.functionPtrType(group_data.ptrType)], group_data);
      this@urx.Object(urx.PtrType.SHARED, idLocal);
    end

    function freeMem(this)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_delete' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function res = sequencesCount(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_sequencesCount' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function res = eventsCount(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_eventsCount' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function res = channelsCount(this, event_idx)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_channelsCount' urx.Object.functionPtrType(this.ptrType)], this.id, event_idx - 1);
    end

    function res = samplesCount(this, event_idx)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_samplesCount' urx.Object.functionPtrType(this.ptrType)], this.id, event_idx - 1);
    end

    function res = offset(this, sequence_idx, event_idx, channel_idx, sample_idx)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_offset' urx.Object.functionPtrType(this.ptrType)], this.id, sequence_idx - 1, event_idx - 1, channel_idx - 1, sample_idx - 1);
    end

    function res = sampleByteSize(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_sampleByteSize' urx.Object.functionPtrType(this.ptrType)], this.id);
    end
  end
end
