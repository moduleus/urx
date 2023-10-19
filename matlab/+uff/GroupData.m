classdef GroupData < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
    raw_data
    size_of_data_type
  end


  methods 
    function this = GroupData()
      this.id_ = MexGroupData('newGroupData');
      this.group_timestamp = MexGroupData('get_group_timestamp',this.id_);
      this.sequence_timestamps = MexGroupData('get_sequence_timestamps',this.id_);
      this.raw_data = MexGroupData('get_raw_data',this.id_);
      this.size_of_data_type = MexGroupData('get_size_of_data_type',this.id_);
    end
    
    function setSeqenceTimestamps(this,value)
        MexGroupData('set_sequence_timestamps',this.id_,value);
        this.sequence_timestamps = MexGroupData('get_sequence_timestamps',this.id_);
    end
    
    function setGroupTimestamp(this,value)
        MexGroupData('set_group_timestamp',this.id_,value);
        this.group_timestamp = MexGroupData('get_group_timestamp',this.id_);
    end
    
    function setRawData(this,value)
        MexGroupData('set_raw_data',this.id_,value);
        this.raw_data = MexGroupData('get_raw_data',this.id_);
    end
    
    function setSizeDataType(this,value)
        MexGroupData('set_size_of_data_type',this.id_,value);
        this.size_of_data_type = MexGroupData('get_size_of_data_type',this.id_);
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
        end
    end
    
  end
end 