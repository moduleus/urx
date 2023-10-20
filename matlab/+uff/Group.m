classdef Group < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    group_timestamp
    sampling_type
    description
  end


  methods 
    function this = Group()
      this.id_ = MexUFF('new_Group');
      this.sampling_type = uff.Group.SamplingType(MexUFF('get_sampling_type',this.id_));
      this.description = MexUFF('get_description',this.id_);
    end

    function delete(this)
      MexUFF('delete_Group', this.id_);
    end

    function setSamplingType(this,value)
        MexUFF('set_sampling_type',this.id_, int32(value));
        this.sampling_type = uff.Group.SamplingType(MexUFF('get_sampling_type',this.id_));
    end
    
    function setDescription(this,value)
        MexUFF('set_description',this.id_,value);
        this.sequence_timestamps = MexUFF('get_description',this.id_);
    end
    
    function a = subsasgn(a,s,b)
        if strcmp(s.subs,'description')
            setDescription(a,b);
        elseif strcmp(s.subs,'sampling_type')
            setSamplingType(a,b);
        end
    end
    
  end
end 