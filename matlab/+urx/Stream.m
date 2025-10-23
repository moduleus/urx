classdef Stream < urx.Object
  methods
    function this = Stream(filename, dataset)
      this@urx.Object(urx.PtrType.SHARED, filename, dataset);
    end

    function freeMem(this)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_delete' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function res = dataset(this)
      res = urx.Dataset(this.libBindingRef.call([strrep(class(this), '.', '_') '_dataset' urx.Object.functionPtrType(this.ptrType)], this.id), urx.PtrType.SHARED);
    end

    function saveToFile(this)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_save_to_file' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function loadFromFile(this)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_load_from_file' urx.Object.functionPtrType(this.ptrType)], this.id);
    end

    function setReaderOptions(this, raw_data_load_policy)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_set_reader_options' urx.Object.functionPtrType(this.ptrType)], this.id, raw_data_load_policy);
    end

    function setWriterOptions(this, chunk_group_data, clean_unusable_data, check_data)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_set_writer_options' urx.Object.functionPtrType(this.ptrType)], this.id, chunk_group_data, clean_unusable_data, check_data);
    end

    function res = createGroupData(this, shared_group, timestamp)
      res = urx.GroupDataStream(urx.PtrType.SHARED, this.libBindingRef.call([strrep(class(this), '.', '_') '_create_group_data' urx.Object.functionPtrType(this.ptrType) '_shared'], this.id, shared_group, timestamp), this);
    end

    function readRawData(this, group_data, shared_raw_data, sequence_urx_raw_data, sequence_h5_raw_data, count)
      this.libBindingRef.call([strrep(class(this), '.', '_') '_read_raw_data' urx.Object.functionPtrType(this.ptrType)], this.id, group_data - 1, shared_raw_data, sequence_urx_raw_data - 1, sequence_h5_raw_data - 1, count);
    end
  end
end
