classdef GroupDataStream < urx.Object
  methods
    function this = GroupDataStream(ptrType, id, parent)
      this@urx.Object(ptrType, id, parent);
    end

    function freeMem(this)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_delete' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function append(this, shared_raw_data, sequence_timestamp, vector_event_timestamp)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_append' urx.Object.functionPtrType(this.ptrType)], this.id, shared_raw_data, sequence_timestamp, vector_event_timestamp, numel(vector_event_timestamp));
    end

    function res = getGroupData(this)
      res = urx.GroupData(this.libBindingRef.call([strrep(class(this), '.', '_') '_get_group_data' urx.Object.functionPtrType(this.ptrType)], this.id), urx.PtrType.RAW);
    end
  end
end
