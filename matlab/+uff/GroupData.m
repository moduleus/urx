classdef GroupData < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
    event_timestamps
    raw_data
    group
  end


  methods 
    function this = GroupData()
      this.id_ = MexUFF('GroupData_new');
      this.group_timestamp = MexUFF('GroupData_get_group_timestamp',this.id_);
      this.sequence_timestamps = MexUFF('GroupData_get_sequence_timestamps',this.id_);
      this.event_timestamps = MexUFF('GroupData_get_event_timestamps',this.id_);
      this.raw_data = MexUFF('GroupData_get_raw_data',this.id_);
      try
        this.group = uff.Group(MexUFF('GroupData_get_group',this.id_));
      catch exceptions
          this.group = [];
      end
    end

    function delete(this)
      MexUFF('GroupData_delete', this.id_);
    end

    function set.raw_data(this,value)
        MexUFF('GroupData_set_raw_data',this.id_,value);
    end

    function rd = get.raw_data(this)
        rd = MexUFF('GroupData_get_raw_data',this.id_);
    end
    
    function set.event_timestamps(this,value)
        MexUFF('GroupData_set_event_timestamps',this.id_,value);
    end
    
    function et = get.event_timestamps(this)
        et = MexUFF('GroupData_get_event_timestamps',this.id_);
    end
    
    function set.group_timestamp(this,value)
        MexUFF('GroupData_set_group_timestamp',this.id_,value);
    end
    
    function gt = get.group_timestamp(this)
        gt = MexUFF('GroupData_get_group_timestamp',this.id_);
    end
    
    function set.group(this,group)
        if ~ isempty(group)
            MexUFF('GroupData_set_group',this.id_,group.id);
        end
    end
    
    function g = get.group(this)
        g_id = MexUFF('GroupData_get_group',this.id_);
        g = uff.Group(g_id);
    end
    
  end
end 