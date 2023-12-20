classdef StdVector < handle
  properties (Access = public)
    libBindingRef = urx.LibBinding.empty(1,0)
    id(1,1) = libpointer
    objectClassName(1,:) char
    nbDims
    type(1,1) urx.StdVectorType
  end
  
  properties (Constant)
    MANAGED_PRIMITIVES_TYPES = {'double'} % TODO: add int32..
  end
  
  methods
    function this = StdVector(objectClassName, nbDims, type)
      this.libBindingRef = urx.LibBinding.getInstance();
      this.objectClassName = objectClassName;
      if nargin < 2
        nbDims = 1;
      end
      this.nbDims = nbDims;
      this.type = type;
    end
    
    function delete(this)
      %if this.isAnAllocatedObject()
      %  this.libBindingRef.call(this.functionName('delete'), this.id);
      %end
    end
    
    function res = functionName(this, fun, owner)
        res = 'vector';
        if this.nbDims > 1
          res = [res '_' this.nbDims 'd'];
        end
        if this.type == urx.StdVectorType.SHARED
          res = [res '_shared'];
        elseif this.type == urx.StdVectorType.WEAK
          res = [res '_weak'];
        end
        res = [res '_' strrep(this.objectClassName, '.', '_') '_' fun];
        if nargin >=3 && owner
          res = [res '_shared'];
        end
    end
    
    function res = objectSizeof(this)
      if this.nbDims > 1
        res = this.libBindingRef.call(['std_vector_' this.objectClassName '_sizeof']);
      elseif ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES))
        res = this.libBindingRef.call([this.objectClassName '_sizeof']);
      end
    end
    
    function clear(this)
      this.libBindingRef.call(this.functionName('clear'), this.id);
    end
    
    function pushBack(this, val)
      if isa(val, 'urx.Object') || this.nbDims > 1
        this.libBindingRef.call(this.functionName('push_back', val.ownerOfMemory), this.id, val.id);
      else
        this.libBindingRef.call(this.functionName('push_back', false), this.id, val);
      end
    end
    
    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = this.libBindingRef.call(this.functionName('size'), this.id);
    end
    
    function res = data(this, i)
      res = urx.(this.objectClassName(5:end))(this.libBindingRef.call(this.functionName('data'), this.id, i-1), this.type == urx.StdVectorType.SHARED);
    end
    
    function copy(this, other)
      this.libBindingRef.call(this.functionName('copy'), this.id, other.id);
    end
  end
end
