classdef Version
  properties (Access = public)
    major (1,1) uint16 
    minor (1,1) uint16
    patch (1,1) uint16
  end


  methods 
    function this = Version(major, minor, patch)
      if nargin >2
        this.major = major;
        this.minor = minor;
        this.patch = patch;
      end
    end
    
    function res = eq(this, other)
      res = isequaln(this,other);
    end
  end
end 