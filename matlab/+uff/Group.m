classdef Group < handle
  properties (Access = public)
    id
  end
  
  properties (Access = public)
    group_timestamp
    sampling_type
    description
  end


  methods 
    function this = Group(id)
      if nargin > 0
        this.id = id; 
      else
        this.id = MexUFF('new_Group');
      end
      this.sampling_type = uff.Group.SamplingType(MexUFF('get_sampling_type',this.id));
      this.description = MexUFF('get_description',this.id);
    end

    function delete(this)
      MexUFF('delete_Group', this.id);
    end

    function setSamplingType(this,value)
        MexUFF('set_sampling_type',this.id, int32(value));
        this.sampling_type = uff.Group.SamplingType(MexUFF('get_sampling_type',this.id));
    end
    
    function st = get.sampling_type(this)
        st = uff.Group.SamplingType(MexUFF('get_sampling_type',this.id));
    end
    
    function setDescription(this,value)
        MexUFF('set_description',this.id,value);
        this.description = MexUFF('get_description',this.id);
    end
    
    function d = get.description(this)
        d = MexUFF('get_description',this.id);
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