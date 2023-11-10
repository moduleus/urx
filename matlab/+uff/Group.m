classdef Group < handle
  properties (Access = public)
    id
  end
  
  properties (Access = public)
    sampling_type
    data_type
    description
  end


  methods 
    function this = Group(id)
      if nargin > 0
        this.id = MexUFF('Group_ref',id); 
      else
        this.id = MexUFF('Group_new');
      end
      this.sampling_type = uff.Group.SamplingType(MexUFF('Group_get_sampling_type',this.id));
      this.data_type = uff.Group.DataType(MexUFF('Group_get_data_type',this.id));
      this.description = MexUFF('Group_get_description',this.id);
    end

    function delete(this)
      MexUFF('Group_delete', this.id);
    end

    function set.sampling_type(this,value)
        MexUFF('Group_set_sampling_type',this.id, int32(value));
    end
    
    function st = get.sampling_type(this)
        st = uff.Group.SamplingType(MexUFF('Group_get_sampling_type',this.id));
    end

    function set.data_type(this,value)
        MexUFF('Group_set_data_type',this.id, int32(value));
    end
    
    function st = get.data_type(this)
        st = uff.Group.DataType(MexUFF('Group_get_data_type',this.id));
    end
    
    function set.description(this,value)
        MexUFF('Group_set_description',this.id,value);
    end
    
    function d = get.description(this)
        d = MexUFF('Group_get_description',this.id);
    end
    
  end
end 