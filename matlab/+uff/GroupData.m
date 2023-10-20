classdef GroupData < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
    raw_data
    size_of_data_type
    data_type
  end


  methods 
    function this = GroupData()
      this.id_ = MexUFF('new_GroupData');
      this.group_timestamp = MexUFF('get_group_timestamp',this.id_);
      this.sequence_timestamps = MexUFF('get_sequence_timestamps',this.id_);
      this.raw_data = MexUFF('get_raw_data',this.id_);
      this.size_of_data_type = MexUFF('get_size_of_data_type',this.id_);
      this.data_type = uff.GroupData.DataType(MexUFF('get_data_type',this.id_));
    end

    function delete(this)
      MexUFF('delete_GroupData', this.id_);
    end
    
    function setSeqenceTimestamps(this,value)
        MexUFF('set_sequence_timestamps',this.id_,value);
        this.sequence_timestamps = MexUFF('get_sequence_timestamps',this.id_);
    end
    
    function setGroupTimestamp(this,value)
        MexUFF('set_group_timestamp',this.id_,value);
        this.group_timestamp = MexUFF('get_group_timestamp',this.id_);
    end
    
    function setRawData(this,value)
        MexUFF('set_raw_data',this.id_,value);
        this.raw_data = MexUFF('get_raw_data',this.id_);
    end
    
    function setSizeDataType(this,value)
        MexUFF('set_size_of_data_type',this.id_, value);
        this.size_of_data_type = MexUFF('get_size_of_data_type',this.id_);
    end
    
    function setDataType(this,value)
        MexUFF('set_data_type',this.id_, int32(value));
        this.data_type = uff.GroupData.DataType(MexUFF('get_data_type',this.id_));
    end
    
    function a = subsasgn(a,s,b)
        if strcmp(s.subs,'sequence_timestamps')
            setSeqenceTimestamps(a,b);
        elseif strcmp(s.subs,'group_timestamp')
            setGroupTimestamp(a,b);
        elseif strcmp(s.subs,'raw_data')
            setRawData(a,b);
        elseif strcmp(s.subs,'size_of_data_type')
            setSizeDataType(a,b);
        elseif strcmp(s.subs,'data_type')
            setDataType(a,b);
        end
    end
    
  end
end 