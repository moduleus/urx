classdef Dataset < handle
  properties (Access = private)
    id_
  end
  
  properties (Access = public)
    version
    acquisition
  end


  methods 
    function this = Dataset()
      this.id_ = MexUFF('new_Dataset');
      [major,minor,patch]=MexUFF('get_Dataset_version',this.id_);
      this.version = uff.Version(major,minor,patch);
      %this.acquisition = MexUFF('get_Dataset_acquisition',this.id_);
    end

    function delete(this)
      MexUFF('delete_Dataset', this.id_);
    end

    function setVersion(this,value)
      MexUFF('set_Dataset_version',this.id_, [value.major, value.minor, value.patch]);
      [major,minor,patch]=MexUFF('get_Dataset_version',this.id_);
      this.version = uff.Version(major,minor,patch);
    end

    function setAcquisition(this,value)
      MexUFF('set_Dataset_acquisition',this.id_, value.id_);
      this.acquisition = MexUFF('get_Dataset_acquisition',this.id_);
    end
    
    function a = subsasgn(a,s,b)
      if strcmp(s.subs,'version')
        setVersion(a,b);
      elseif strcmp(s.subs,'acquisition')
        setAcquisition(a,b);
      end
    end
    
  end
end 