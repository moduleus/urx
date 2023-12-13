classdef Acquisition < handle
  properties (Access = public)
    id
  end
  
  properties (Access = public)
      groups = []
  end


  methods 
    function this = Acquisition()
      this.id = MexURX('Acquisition_new');
    end

    function delete(this)
      MexURX('Acquisition_delete', this.id);
    end

    function delGroup(this, group_idx)
      MexURX('Acquisition_del_i_group', this.id, group_idx-1);
      this.groups(group_idx) = [];
    end

    function grp = getGroup(this, group_idx)
      grp = MexURX('Acquisition_get_i_group', this.id, group_idx-1);
    end

    function addGroup(this, group)
      if nargin > 1
        MexURX('Acquisition_add_group', this.id, group.id);
      else
        MexURX('Acquisition_add_new_group', this.id);
      end
      this.groups(end+1) = this.getGroup(length(this.groups)+1);
    end
    
    function a = subsasgn(a,s,b)
    end
    
  end
end 