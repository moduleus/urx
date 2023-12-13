classdef StdVector < handle
  properties (Access = public)
    libBindingRef = uff.LibBinding.empty(1,0)
    id(1,1) = libpointer                              % std_vector base pter
    parent {mustBeScalarOrEmpty} % parent (enable to hold the pointed memory)
    parentPptName
    objects       % matlab uff.Object list (not used for primitive type)
    objectClassName(1,:) char
    nbDims
  end

  properties (Constant)
    MANAGED_PRIMITIVES_TYPES = {'double'} % TODO: add int32..
  end

  methods
    function this = StdVector(objectClassName, parent, parentPptNameOrDataIdx, nbDims)
      this.libBindingRef = uff.LibBinding.getInstance();
      this.objectClassName = objectClassName;
      if nargin < 4
        nbDims = 1;
      end
      this.nbDims = nbDims;
      if nargin < 2 || isempty(parent)
        this.id = uff.LibBinding.call([this.className() '_new']);
        return;
      end
      if ~isempty(parent)
        this.updateParent(parent, parentPptNameOrDataIdx);
      end
    end

    function delete(this)
      if this.isAnAllocatedObject()
        uff.LibBinding.call([this.className() '_delete'], this.id);
      end
    end

    function updateParent(this, parent, parentPptNameOrDataIdx)
      if ischar(parentPptNameOrDataIdx)
        this.parentPptName = parentPptNameOrDataIdx;
        ptrGetFunction = [parent.className() '_' this.parentPptName];
        this.id = uff.LibBinding.call(ptrGetFunction, parent.id);
      else % std_vector
        dataPtr = parent.data();
        len = parent.len();
        stdVecSizeof = uff.LibBinding.call(['std_vector_' this.objectClassName '_sizeof']);
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
        res = [res '_uff'];
      end
      res = [res '_' this.objectClassName];
    end

    function res = objectSizeof(this)
      if this.nbDims > 1
        res = uff.LibBinding.call(['std_vector_' this.objectClassName '_sizeof']);
      elseif ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES))
        res = uff.LibBinding.call([this.objectClassName '_sizeof']);
      end
    end

    function res = isAnAllocatedObject(this)
      res = isempty(this.parent);
    end

    function clear(this)
      uff.LibBinding.call([this.className() '_clear'], this.id);
    end

    function pushBack(this, val)
      if ~any(strcmp(this.objectClassName, this.MANAGED_PRIMITIVES_TYPES)) || this.nbDims > 1
        uff.LibBinding.call([this.className() '_push_back'], this.id, val.id);
      else
        uff.LibBinding.call([this.className() '_push_back'], this.id, val);
      end
    end

    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = uff.LibBinding.call([this.className() '_size' ], this.id);
    end

    function res = data(this)
      res = uff.LibBinding.call([this.className() '_data'], this.id);
    end

    function copy(this, other)
      uff.LibBinding.call([this.className() '_copy'], this.id, other.id);
    end

    %% uff specific
    function updateFromCpp(this)
      objectsBasePtr = this.data();
      sizeofObject = this.objectSizeof();
      nbObject = this.len();
      objectsBasePtr.setdatatype('uint8Ptr', nbObject * sizeofObject);
      for i=1:nbObject
        if numel(this.objects) < i
          if this.nbDims == 1
            this.objects(i) = uff.(this.objectClassName)(this, int32(i - 1), ...
                                                         objectsBasePtr + (i-1) * sizeofObject)
          else
            if isempty(this.objects)
              this.objects = uff.StdVector(this.objectClassName, this, i, 1);
            else
              this.objects(i) = uff.StdVector(this.objectClassName, this, i, 1);
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
              parentPptName = uff.Object.camelToSnakeCase(stdVectorParentPptName(10:end));
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
          stdVec = uff.StdVector(this.objectClassName, this, i);
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
            stdVec = uff.StdVector(this.objectClassName);
            stdVec.setToCpp(v{i});
            this.pushBack(stdVec);
          end
        else
          this.objectClassName
          for i=1:size(v, 2)
            stdVec = uff.StdVector(this.objectClassName);
            stdVec.setToCpp(v(i,:));
            this.pushBack(stdVec);
          end
        end
      end
    end
  end
end
