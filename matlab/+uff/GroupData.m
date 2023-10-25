classdef GroupData < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
    event_timestamps
    raw_data
    size_of_data_type
    data_type
    group
  end


  methods 
    function this = GroupData()
      this.id_ = MexUFF('GroupData_new');
      this.group_timestamp = MexUFF('GroupData_get_group_timestamp',this.id_);
      this.sequence_timestamps = MexUFF('GroupData_get_sequence_timestamps',this.id_);
      this.event_timestamps = MexUFF('GroupData_get_event_timestamps',this.id_);
      this.raw_data = MexUFF('GroupData_get_raw_data',this.id_);
      this.size_of_data_type = MexUFF('GroupData_get_size_of_data_type',this.id_);
      this.data_type = uff.GroupData.DataType(MexUFF('GroupData_get_data_type',this.id_));
    end

    function delete(this)
      MexUFF('GroupData_delete', this.id_);
    end
    
    function setSeqenceTimestamps(this,value)
        MexUFF('GroupData_set_sequence_timestamps',this.id_,value);
        this.sequence_timestamps = MexUFF('GroupData_get_sequence_timestamps',this.id_);
    end
    
    function setEventTimestamps(this,value)
        MexUFF('GroupData_set_event_timestamps',this.id_,value);
        this.event_timestamps = MexUFF('GroupData_get_event_timestamps',this.id_);
    end
    
    function setGroupTimestamp(this,value)
        MexUFF('GroupData_set_group_timestamp',this.id_,value);
        this.group_timestamp = MexUFF('GroupData_get_group_timestamp',this.id_);
    end
    
    function setRawData(this,value)
        MexUFF('GroupData_set_raw_data',this.id_,value);
        this.raw_data = MexUFF('GroupData_get_raw_data',this.id_);
    end
    
    function setSizeDataType(this,value)
        MexUFF('GroupData_set_size_of_data_type',this.id_, value);
        this.size_of_data_type = MexUFF('GroupData_get_size_of_data_type',this.id_);
    end
    
    function setDataType(this,value)
        MexUFF('GroupData_set_data_type',this.id_, int32(value));
        this.data_type = uff.GroupData.DataType(MexUFF('GroupData_get_data_type',this.id_));
    end
    
    function setGroup(this,group,acq)
        MexUFF('GroupData_set_group',this.id_,group.id,acq.id);
        this.group = uff.Group(MexUFF('GroupData_get_group',this.id_));
    end
    
    function a = subsasgn(a,s,b)
        if strcmp(s.subs,'sequence_timestamps')
            setSeqenceTimestamps(a,b);
        elseif strcmp(s.subs,'event_timestamps')
            setEventTimestamps(a,b);
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