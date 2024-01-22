classdef StdVector < urx.ObjectField
  properties (Access = public)
    objectClassName(1,:) char
    nbDims uint8
    parent urx.Object {mustBeScalarOrEmpty} = urx.Object.empty(1,0)
  end
  
  methods
    function this = StdVector(objectClassName, nbDims, ptrType, parent)
      this.libBindingRef = urx.LibBinding.getInstance();
      this.objectClassName = objectClassName;
      if nargin < 2
        nbDims = 1;
      end
      this.nbDims = nbDims;
      this.ptrType = ptrType;
      if nargin < 4
        this.id = this.libBindingRef.call(this.functionName('new'));
      else
        this.parent = parent;
      end
    end
    
    function delete(this)
      %if this.isAnAllocatedObject()
      %  this.libBindingRef.call(this.functionName('delete'), this.id);
      %end
    end
    
    function res = functionName(this, fun, owner)
        res = 'vector';
        if this.nbDims > 1
          res = [res '_' int2str(this.nbDims) 'd'];
        end
        if this.ptrType == urx.PtrType.SHARED
          res = [res '_shared'];
        elseif this.ptrType == urx.PtrType.WEAK
          res = [res '_weak'];
        end
        if regexp(this.objectClassName,'[u]?int\d+')
          ctype = [this.objectClassName '_t'];
        else
          ctype = this.objectClassName;
        end
        res = [res '_' strrep(ctype, '.', '_') '_' fun];
        if nargin >=3 && owner
          res = [res '_shared'];
        end
    end
    
    function res = objectSizeof(this)
      res = this.libBindingRef.call(this.functionName('sizeof'));
    end
    
    function clear(this)
      this.libBindingRef.call(this.functionName('clear'), this.id);
    end
    
    function pushBack(this, val)
      if this.nbDims > 1
        this.libBindingRef.call(this.functionName('push_back'), this.id, val.id);
      elseif isa(val, 'urx.Object') || this.nbDims > 1
        this.libBindingRef.call(this.functionName('push_back', val.ptrType == urx.PtrType.SHARED), this.id, val.id);
      else
        this.libBindingRef.call(this.functionName('push_back'), this.id, val);
      end
    end
    
    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = this.libBindingRef.call(this.functionName('size'), this.id);
    end
    
    function res = data(this, i)
      if this.nbDims > 1
        res = urx.StdVector(this.objectClassName, this.nbDims-1, this.ptrType, this.parent);
        res.id = this.libBindingRef.call(this.functionName('data'), this.id, i-1);
      elseif strcmp(this.objectClassName(1:4), 'urx.')
        res = urx.(this.objectClassName(5:end))(this.libBindingRef.call(this.functionName('data'), this.id, i-1), this.ptrType, this.parent);
      else
        resPtr = this.libBindingRef.call(this.functionName('data'), this.id, i-1);
        resPtr.setdatatype([this.objectClassName 'Ptr'], 1);
        res = resPtr.Value;
      end
    end
    
    function copy(this, other)
      this.libBindingRef.call(this.functionName('copy'), this.id, other.id);
    end
  end
end
