classdef GroupData < handle
  properties (Access = public)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sequence_timestamps
  end


  methods 
    function this = GroupData()
      this.id_ = MexGroupData('newGroupData');
      this.group_timestamp = MexGroupData('get_group_timestamp',this.id_);
      this.sequence_timestamps = MexGroupData('get_sequence_timestamps',this.id_);
    end
    
    function setSeqTimestamp(this,value)
        this.sequence_timestamps = MexGroupData('set_sequence_timestamps',this.id_,value);
    end
    function a = subsasgn(a,s,b)
        if strcmp(s.subs,'get_sequence_timestamps')
            setSeqTimestamp(a,b);
        end
    end

    function create(this)
      MexGroupData('create', this.id_);
    end

    function printData(this)
      MexGroupData('printData', this.id_);
    end
  end
end 