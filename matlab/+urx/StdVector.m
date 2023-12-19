classdef StdVector < handle
  properties (Access = public)
    libBindingRef = urx.LibBinding.empty(1,0)
    id(1,1) = libpointer                              % std_vector base pter
    parent {mustBeScalarOrEmpty} % parent (enable to hold the pointed memory)
    parentPptName
    objects       % matlab urx.Object list (not used for primitive type)
    objectClassName(1,:) char
    nbDims
  end
  
  properties (Constant)
    MANAGED_PRIMITIVES_TYPES = {'double'} % TODO: add int32..
  end
  
  methods
    function this = StdVector(objectClassName, parent, parentPptNameOrDataIdx, nbDims)
      this.libBindingRef = urx.LibBinding.getInstance();
      this.objectClassName = objectClassName;
      if nargin < 4
        nbDims = 1;
      end
      this.nbDims = nbDims;
      if nargin < 2 || isempty(parent)
        this.id = this.libBindingRef.call([this.className() '_new']);
        return;
      end
      if ~isempty(parent)
        this.updateParent(parent, parentPptNameOrDataIdx);
      end
    end
    
    function delete(this)
      if this.isAnAllocatedObject()
        this.libBindingRef.call([this.className() '_delete'], this.id);
      end
    end
    
    function updateParent(this, parent, parentPptNameOrDataIdx)
      if ischar(parentPptNameOrDataIdx)
        this.parentPptName = parentPptNameOrDataIdx;
        ptrGetFunction = [strrep(class(parent), '.', '_') '_' this.parentPptName];
        this.id = this.libBindingRef.call(ptrGetFunction, parent.id);
      else % std_vector
        dataPtr = parent.data();
        len = parent.len();
        stdVecSizeof = this.libBindingRef.call(['std_vector_' this.objectClassName '_sizeof']);
        if len > 0
          dataPtr.setdatatype('uint8Ptr', stdVecSizeof * len);
          this.id = dataPtr + (parentPptNameOrDataIdx-1) * stdVecSizeof;
        else
          this.id = dataPtr;
        end
      end
      this.parent = parent;
    end
    
    function res = className(this)
      res = 'std_vector';
      if this.nbDims > 1
        res = [res '_2d'];
      end
      if ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES))
        res = [res '_urx'];
      end
      res = [res '_' this.objectClassName];
    end
    
    function res = objectSizeof(this)
      if this.nbDims > 1
        res = this.libBindingRef.call(['std_vector_' this.objectClassName '_sizeof']);
      elseif ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES))
        res = this.libBindingRef.call([this.objectClassName '_sizeof']);
      end
    end
    
    function res = isAnAllocatedObject(this)
      res = isempty(this.parent);
    end
    
    function clear(this)
      this.libBindingRef.call([this.className() '_clear'], this.id);
    end
    
    function pushBack(this, val)
      if ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES)) || this.nbDims > 1
        this.libBindingRef.call([this.className() '_push_back'], this.id, val.id);
      else
        this.libBindingRef.call([this.className() '_push_back'], this.id, val);
      end
    end
    
    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = this.libBindingRef.call([this.className() '_size' ], this.id);
    end
    
    function res = data(this)
      res = this.libBindingRef.call([this.className() '_data'], this.id);
    end
    
    function copy(this, other)
      this.libBindingRef.call([this.className() '_copy'], this.id, other.id);
    end
    
    %% urx specific
    function updateFromCpp(this)
      objectsBasePtr = this.data();
      sizeofObject = this.objectSizeof();
      nbObject = this.len();
      objectsBasePtr.setdatatype('uint8Ptr', nbObject * sizeofObject);
      for i=1:nbObject
        if numel(this.objects) < i
          if this.nbDims == 1
            this.objects(i) = urx.(this.objectClassName)(this, int32(i - 1), ...
              objectsBasePtr + (i-1) * sizeofObject)
          else
            if isempty(this.objects)
              this.objects = urx.StdVector(this.objectClassName, this, i, 1);
            else
              this.objects(i) = urx.StdVector(this.objectClassName, this, i, 1);
            end
          end
        else
          this.objects(i).id = objectsBasePtr + (i-1) * sizeofObject;
          this.objects(i).containerId = int32(i - 1);
          this.objects(i).container = this;
          objectsPpt = properties(this.objects);
          objectsPptStdVecInd = find(strncmp(objectsPpt, 'stdVector', 9));
          for i=1:numel(objectsPptStdVecInd)
            for j=1:numel(this.objects)
              parent = this.objects(i);
              stdVectorParentPptName = objectsPpt{objectsPptStdVecInd(i)};
              parentPptName = urx.Object.camelToSnakeCase(stdVectorParentPptName(10:end));
              this.objects(j).(stdVectorParentPptName).updateParent(this.objects(i), parentPptName);
            end
          end
        end
      end
      if numel(this.objects) > nbObject
        this.objects(nbObject:end) = [];
      end
    end
    
    %% primivite types specific
    function res = getFromCpp(this)
      dataBasePtr = this.data();
      nbObject = this.len();
      if this.nbDims == 1
        if nbObject == 0
          res = eval([this.objectClassName '([])']);
          return
        end
        dataBasePtr.setdatatype([this.objectClassName 'Ptr'], nbObject);
        res = dataBasePtr.Value;
      else
        if nbObject == 0
          res = eval(['{' this.objectClassName '([])}']);
          return
        end
        for i=1:this.len()
          stdVec = urx.StdVector(this.objectClassName, this, i);
          res{i} = stdVec.getFromCpp();
        end
      end
    end
    
    function setToCpp(this, v)
      this.clear();
      if this.nbDims == 1
        for i=1:numel(v)
          this.pushBack(v(i));
        end
      else
        if isa(v, 'cell')
          for i=1:numel(v)
            stdVec = urx.StdVector(this.objectClassName);
            stdVec.setToCpp(v{i});
            this.pushBack(stdVec);
          end
        else
          this.objectClassName
          for i=1:size(v, 2)
            stdVec = urx.StdVector(this.objectClassName);
            stdVec.setToCpp(v(i,:));
            this.pushBack(stdVec);
          end
        end
      end
    end
  end
end
