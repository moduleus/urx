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
        this.id = MexURX('Group_ref',id); 
      else
        this.id = MexURX('Group_new');
      end
      this.sampling_type = urx.Group.SamplingType(MexURX('Group_get_sampling_type',this.id));
      this.data_type = urx.Group.DataType(MexURX('Group_get_data_type',this.id));
      this.description = MexURX('Group_get_description',this.id);
    end

    function delete(this)
      MexURX('Group_delete', this.id);
    end

    function set.sampling_type(this,value)
        MexURX('Group_set_sampling_type',this.id, int32(value));
    end
    
    function st = get.sampling_type(this)
        st = urx.Group.SamplingType(MexURX('Group_get_sampling_type',this.id));
    end

    function set.data_type(this,value)
        MexURX('Group_set_data_type',this.id, int32(value));
    end
    
    function st = get.data_type(this)
        st = urx.Group.DataType(MexURX('Group_get_data_type',this.id));
    end
    
    function set.description(this,value)
        MexURX('Group_set_description',this.id,value);
    end
    
    function d = get.description(this)
        d = MexURX('Group_get_description',this.id);
    end
    
  end
end 