classdef Object < handle

  properties (Access = public)
    libBindingRef = urx.LibBinding.empty(1,0)
    id(1,1) = libpointer
    ptrType urx.PtrType
    saveId
    savePtrType urx.PtrType
    parent
  end

  methods
    % Constructor for urx object.

    % User should only use it with empty argument or set only varargin:
    % urx.RawData_double_real([], [], [], 5)

    % varargin are parameters passed to the constructor. If set, id,
    % ptrType and parent are ignored.

    % parent is used when an object is inside another object. For example:
    % Acquisition is inside a Dataset object. This feature should not be
    % used by user.
    function this = Object(id, ptrType, parent, varargin)
      this.libBindingRef = urx.LibBinding.getInstance();

      if nargin == 0
        this.id = this.libBindingRef.call([strrep(class(this), '.', '_') '_new']);
        this.ptrType = urx.PtrType.SHARED;
      elseif nargin == 3 % Only used from PreGet event.
        this.id = id;
        this.ptrType = ptrType;
        this.parent = parent;
      elseif nargin == 4
        this.id = this.libBindingRef.call([strrep(class(this), '.', '_') '_new'], varargin{:});
        this.ptrType = urx.PtrType.SHARED;
      else
        assert(false);
      end
      mc = metaclass(this);
      props = mc.PropertyList;
      for i = 1:numel(props)
        if props(i).SetObservable
          if (isa(this.(props(i).Name), 'urx.Object'))
            addlistener(this, props(i).Name, 'PreSet', @urx.Object.handlePropEvents);
          end
          addlistener(this, props(i).Name, 'PostSet', @urx.Object.handlePropEvents);
        end
        if props(i).GetObservable
          addlistener(this, props(i).Name, 'PreGet', @urx.Object.handlePropEvents);
        end
      end
    end

    function freeMem(this)
      % If the object has no parent, the C shared_ptr associted must be
      % released.
      if isempty(this.parent)
        deleteFunction = [strrep(class(this), '.', '_') '_delete'];
        this.libBindingRef.call(deleteFunction, this.id);
        this.id = libpointer;
      end
    end

    function delete(this)
      this.freeMem();
    end

    % May be useful to ease debug.
    function res = showPtr(this, ptr)
      res = uint64(this.libBindingRef.call('get_pointer', ptr));
    end
  end

  methods (Static)
    function res = functionAssign(className, typeDest, typeSrc)
      res = [className '_assign'];
      if typeDest == urx.PtrType.SHARED
        res = [res '_shared'];
      elseif typeDest == urx.PtrType.WEAK
        res = [res '_weak'];
      else
        res = [res '_raw'];
      end
      if typeSrc
        res = [res '_shared'];
      else
        res = [res '_raw'];
      end
    end
    
    function res = functionVector(className, func, type, nbDims)
      res = 'vector';
      if type == urx.PtrType.SHARED
        res = [res '_shared'];
      elseif type == urx.PtrType.WEAK
        res = [res '_weak'];
      end
      if nbDims == 2
        res = [res '_2d'];
      end
      res = [res '_' className '_' func];
    end
    
    function handlePropEvents(src,evnt)
      % Since this function read properties from urx object, it may be
      % needed to disable Get/Set Recution.
      persistent disableSetRecursion;
      if isempty(disableSetRecursion)
        disableSetRecursion = 0;
      end
      persistent disableGetRecursion;
      if isempty(disableGetRecursion)
        disableGetRecursion = 0;
      end

      % get stack to eventually avoid recursion and side effects.
      s = dbstack;

      % Security
      if numel(s) == 1
        assert(disableSetRecursion == 0);
        assert(disableGetRecursion == 0);
      end

      % Disable recursion for Set.
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents') && disableSetRecursion > 0 && ...
          (strcmp(evnt.EventName, 'PreSet') || strcmp(evnt.EventName, 'PostSet'))
        return;
      end
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents') && disableGetRecursion > 0 && ...
          strcmp(evnt.EventName, 'PreGet')
        return;
      end

      % Disable hint for tipinfo and workspace info. To ease debug. Need to
      % be removed before release.
      if any(arrayfun(@(x) strcmp(x.name, 'datatipinfo'), s)) || any(arrayfun(@(x) strcmp(x.name, 'workspacefunc'), s))
        % disp will be shown in tipinfo.
        disp(['Disabled ' evnt.EventName ' for ' src.Name '. Shown value may be wrong.'])
        return;
      end

      % Don't observe when called from constructor with inheritance.
      % Object constructor must be called before child constructor.
      % So all properties already have listeners set.
      for i = 1:numel(s)
        strInheritance = strfind(s(i).name, '.');
        % Try to found in the stack "Name.Name".
        if numel(strInheritance) == 1 && strcmp(s(i).name, [s(i).name(1:strInheritance-1) '.' s(i).name(1:strInheritance-1)])
          return;
        end
      end

      % Get data from event.
      affectedObject = evnt.AffectedObject;
      affectedPropertyName = src.Name;
      if any(strcmp(properties(affectedObject), [affectedPropertyName 'Std']))
        affectedPropertyStd = affectedObject.([affectedPropertyName 'Std']);
      else
        affectedPropertyStd = [];
      end
      disableGetRecursion = disableGetRecursion + 1;
      % Get current cached value of the property.
      % Get must be disabled to avoid recursion.
      affectedProperty = affectedObject.(affectedPropertyName);
      disableGetRecursion = disableGetRecursion - 1;
      % If no value has been set, you need to force update to get value and
      % to set id to C pointer.
      if isempty(affectedProperty) && strcmp(evnt.EventName, 'PreSet')
        % Wihout disableGetRecursion, it will generate an PreGet event.
        affectedProperty = affectedObject.(affectedPropertyName);
      end

      % For update of the C pointer at every call to be sure that value is
      % up to date.
      % Maybe only isempty(affectedProperty)
      functionCFieldAccessor = strrep(class(affectedObject), '.', '_');
      if affectedObject.ptrType == urx.PtrType.SHARED
        functionCFieldAccessor = [functionCFieldAccessor '_shared'];
      elseif affectedObject.ptrType == urx.PtrType.WEAK
        functionCFieldAccessor = [functionCFieldAccessor '_weak'];
      end
      functionCFieldAccessor = [functionCFieldAccessor '_' urx.Object.camelToSnakeCase(affectedPropertyName)];
      libBindingRef = urx.LibBinding.getInstance();
      affectedCFieldPtr = libBindingRef.call(functionCFieldAccessor, affectedObject.id);

      affectedPropertyClassName = class(affectedProperty);

      % decision tree: event then, property class (char, double, int32/enum or urx.* from std_vector)
      switch evnt.EventName
        % Save important data before Matlab erase the old value by the new
        % one.
        % PostSet will need them to update urx object in C data.
        case 'PreSet'
          if isa(affectedProperty, 'urx.Object') && isempty(affectedPropertyStd)
            affectedObject.saveId = affectedProperty.id;
            affectedObject.savePtrType = affectedProperty.ptrType;
          end

        case 'PostSet'
          if ischar(affectedProperty)
            libBindingRef.call('std_string_set', affectedCFieldPtr, affectedProperty);
          elseif isenum(affectedProperty)
            assert(numel(affectedProperty) == 1);
            affectedCFieldPtr.setdatatype('int32Ptr', 1);
            affectedCFieldPtr.Value = int32(affectedProperty);
            % Not a stdVector
          elseif isempty(affectedPropertyStd)
            % Native type.
            if ~isa(affectedProperty, 'urx.Object')
              if numel(affectedProperty) ~= 1 && (~strncmp(class(affectedObject), 'urx.RawData', strlength('urx.RawData')) || ~strcmp(affectedPropertyName, 'data'))
                throw(MException('urx:fatalError', 'Only single value is supported.'));
              end
              affectedCFieldPtr.setdatatype([affectedPropertyClassName 'Ptr'], size(affectedProperty, 1), size(affectedProperty, 2));
              affectedCFieldPtr.Value = affectedProperty;
              % urx.Object
            else

              % When you assign an urx object to another one, you don't
              % assign pointer, you copy data from a pointer to another
              % pointer.
              % So after assigning new data to old data, you need to free new
              % data (that has been copied to old data).
              assignFunction = urx.Object.functionAssign(strrep(affectedPropertyClassName, '.', '_'), affectedObject.savePtrType, affectedProperty.ptrType);
              libBindingRef.call(assignFunction, affectedCFieldPtr, affectedProperty.id);

              affectedProperty.freeMem();

              % Restore pointer and ptrType of the property.
              affectedProperty.id = affectedObject.saveId;
              affectedProperty.ptrType = affectedObject.savePtrType;

              % The property must remember the parent. You may want to delete
              % the parent object and want to use the object property. I.e.
              % dataset = urx.Dataset();
              % version = urx.Version();
              % version.minor = 111;
              % dataset.version = version;
              % clear dataset
              % Here, version variable must be usable even if dataset
              % (and dataset.version) is cleared.
              affectedProperty.parent = affectedObject;
            end
          else
            affectedPropertyStd.id = affectedCFieldPtr;
            affectedPropertyStd.clear();
            % Check consistency.
            if affectedPropertyStd.nbDims == 1
              assert(strcmp(class(affectedProperty), affectedPropertyStd.objectClassName));
              for i = 1:numel(affectedProperty)
                if isa(affectedProperty(i), 'urx.Object')
                  affectedProperty(i).parent = affectedObject;
                end
                affectedPropertyStd.pushBack(affectedProperty(i));
              end
            else
              assert(affectedPropertyStd.nbDims == 2);
              assert(iscell(affectedProperty));
              for i = 1:numel(affectedProperty)
                vectori = urx.StdVector(affectedPropertyStd.objectClassName, affectedPropertyStd.nbDims-1, affectedPropertyStd.ptrType);
                for j = 1:numel(affectedProperty{i})
                  vectori.pushBack(affectedProperty{i}(j));
                end
                affectedPropertyStd.pushBack(vectori);
              end
            end
          end

          % Before every get:
          %   - if the property is a native type: the value must be updated,
          %   - if the property is an object (C or urx), the pointer of the C
          %       object must be updated.
        case 'PreGet'
          % Disable recursion because all
          % affectedObject.(affectedPropertyName) will generate an
          % PreSet/PostSet.
          disableSetRecursion = disableSetRecursion + 1;
          if ischar(affectedProperty)
            affectedObject.(affectedPropertyName) = libBindingRef.call('std_string_get', affectedCFieldPtr);
          elseif isenum(affectedProperty)
            assert(numel(affectedProperty) == 1);
            affectedCFieldPtr.setdatatype('int32Ptr', 1);
            affectedCFieldPtr.Value = int32(affectedProperty);
          elseif ~isa(affectedProperty, 'urx.Object') && isempty(affectedPropertyStd)
            % RawData.data is the only property that have an array of an
            % not urx.Object.
            if strncmp(class(affectedObject), 'urx.RawData', strlength('urx.RawData')) && strcmp(affectedPropertyName, 'data')
              strSplit = split(class(affectedObject), '_');
              if strcmp(strSplit(end), 'real')
                d2dim = 1;
              else
                d2dim = 2;
              end
              affectedCFieldPtr.setdatatype([strSplit{2} 'Ptr'], d2dim, affectedObject.size);
            else
              assert(numel(affectedProperty) == 1);
              affectedCFieldPtr.setdatatype([affectedPropertyClassName 'Ptr'], 1);
            end

            affectedObject.(affectedPropertyName) = affectedCFieldPtr.Value;
          elseif isempty(affectedPropertyStd) % urx.Object
            % Ugly but to get data from a pointer, You need to know if you
            % are manipulating shared_ptr, weak_ptr or raw pointer.
            if any(strcmp({'rawData'}, affectedPropertyName))
              ptrIsShared = urx.PtrType.SHARED;
            elseif any(strcmp({'elementGeometry','impulseResponse','group','probe','wave'},affectedPropertyName))
              ptrIsShared = urx.PtrType.WEAK;
            else
              ptrIsShared = urx.PtrType.RAW;
            end
            if isempty(affectedObject.(affectedPropertyName))
              affectedObject.(affectedPropertyName) = urx.(affectedPropertyClassName(5:end))(affectedCFieldPtr, ptrIsShared, affectedObject);
            else
              % If object has already been cached, check if nothing has
              % changed.
              assert(affectedObject.showPtr(affectedObject.(affectedPropertyName).id) == affectedObject.showPtr(affectedCFieldPtr));
              assert(affectedObject.(affectedPropertyName).ptrType == ptrIsShared);
            end
          end
          disableSetRecursion = disableSetRecursion - 1;
      end
    end
  end

  methods (Static)
    function res = snakeToCamelCase(str)
      res = [upper(str(1)) regexprep(str(2:end), '_(\w)', '${upper($1)}')];
    end
    function res = camelToSnakeCase(str)
      res = lower(regexprep(str, '(?<=\w)([A-Z])', '_$1'));
    end
  end
end
