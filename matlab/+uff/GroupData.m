classdef GroupData < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
    event_timestamps
    raw_data
    void_raw_data
    void_raw_data_length
    size_of_data_type
    data_type
    group
    vrd
  end


  methods 
    function this = GroupData()
      this.id_ = MexUFF('GroupData_new');
      this.group_timestamp = MexUFF('GroupData_get_group_timestamp',this.id_);
      this.sequence_timestamps = MexUFF('GroupData_get_sequence_timestamps',this.id_);
      this.event_timestamps = MexUFF('GroupData_get_event_timestamps',this.id_);
      this.raw_data = MexUFF('GroupData_get_raw_data',this.id_);
      this.void_raw_data = MexUFF('GroupData_get_void_raw_data',this.id_);
      this.void_raw_data_length = MexUFF('GroupData_get_void_raw_data_length',this.id_);
      this.size_of_data_type = MexUFF('GroupData_get_size_of_data_type',this.id_);
      this.data_type = uff.GroupData.DataType(MexUFF('GroupData_get_data_type',this.id_));
    end

    function delete(this)
      MexUFF('GroupData_delete', this.id_);
    end
    
    function set.sequence_timestamps(this,value)
       MexUFF('GroupData_set_sequence_timestamps',this.id_,value);
    end
    
    function st = get.sequence_timestamps(this,value)
       st=MexUFF('GroupData_get_sequence_timestamps',this.id_);
    end
    
    function set.event_timestamps(this,value)
       MexUFF('GroupData_set_event_timestamps',this.id_,value);
    end
    
    function et = get.event_timestamps(this,value)
       et = MexUFF('GroupData_get_event_timestamps',this.id_);
    end
    
    function set.group_timestamp(this,value)
       MexUFF('GroupData_set_group_timestamp',this.id_,value);
    end
    
    function gt = get.group_timestamp(this,value)
       gt = MexUFF('GroupData_get_group_timestamp',this.id_);
    end
    
    function set.raw_data(this,value)
       MexUFF('GroupData_set_raw_data',this.id_,value);
    end
    
    function rd = get.raw_data(this,value)
       rd = MexUFF('GroupData_get_raw_data',this.id_);
    end
    
    function set.void_raw_data(this,value)
       MexUFF('GroupData_set_void_raw_data',this.id_,value);
    end
    
    function vrd = get.void_raw_data(this,value)
       this.vrd = MexUFF('GroupData_get_void_raw_data',this.id_);
       vrd = this.vrd;
    end
    
    function set.void_raw_data_length(this,value)
    end
    
    function vrdl = get.void_raw_data_length(this,value)
       vrdl = MexUFF('GroupData_get_void_raw_data_length',this.id_);
    end
    
    function set.size_of_data_type(this,value)
       MexUFF('GroupData_set_size_of_data_type',this.id_, value);
    end
    
    function size_of_data_type = get.size_of_data_type(this,value)
       size_of_data_type = MexUFF('GroupData_get_size_of_data_type',this.id_);
    end
    
    function set.data_type(this,value)
       MexUFF('GroupData_set_data_type',this.id_, int32(value));
    end
    
    function data_type = get.data_type(this,value)
       data_type = uff.GroupData.DataType(MexUFF('GroupData_get_data_type',this.id_));
    end
    
    function set.group(this,group,acq)
       MexUFF('GroupData_set_group',this.id_,group.id,acq.id);
    end

    function group = get.group(this)
       group = uff.Group(MexUFF('GroupData_get_group',this.id_));
    end
    
  end
end 