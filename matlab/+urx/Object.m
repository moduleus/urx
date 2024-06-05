classdef Object < urx.ObjectField
  properties (Access = private)
    parent {mustBeScalarOrEmpty} = urx.Object.empty(1,0)
    saveId
    savePtrType urx.PtrType
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
      % Need to skip when initializing a matrix with a none zero size.
      if nargin == 1 && isempty(id)
        return;
      end
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
        % Initialize *Std properties.
        if any(arrayfun(@(x) strcmp(x.Name, [props(i).Name 'Std']), props)) && ~iscell(this.(props(i).Name))
          if any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.sharedPtrInCpp'), props(i).Validation.ValidatorFunctions))
            stdPtrType = urx.PtrType.SHARED;
          elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.optionalInCpp'), props(i).Validation.ValidatorFunctions))
            stdPtrType = urx.PtrType.OPTIONAL;
          elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.weakPtrInCpp'), props(i).Validation.ValidatorFunctions))
            stdPtrType = urx.PtrType.WEAK;
          elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.rawInCpp'), props(i).Validation.ValidatorFunctions))
            stdPtrType = urx.PtrType.RAW;
          elseif ~isa(this.(props(i).Name), 'urx.Object')
            stdPtrType = urx.PtrType.RAW;
          else
            assert(false);
          end
          this.([props(i).Name 'Std']) = urx.StdVector(class(this.(props(i).Name)), 1, stdPtrType, this);
        end
      end
    end

    function freeMem(this)
      % If the object has no parent, the C shared_ptr associated must be
      % released.
      if isempty(this.parent) && ~isempty(this.ptrType) && this.ptrType == urx.PtrType.SHARED
        deleteFunction = [strrep(class(this), '.', '_') '_delete'];
        this.libBindingRef.call(deleteFunction, this.id);
        this.id = libpointer;
      end
    end

    function delete(this)
      this.freeMem();
    end
  end

  methods (Static)
    function res = functionAssign(className, typeDest, typeSrc)
      res = [className '_assign'];
      if typeDest == urx.PtrType.SHARED
        res = [res '_shared'];
      elseif typeDest == urx.PtrType.WEAK
        res = [res '_weak'];
      elseif typeDest == urx.PtrType.OPTIONAL
        res = [res '_optional'];
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
      elseif type == urx.PtrType.OPTIONAL
        res = [res '_optional'];
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
      if isa(affectedObject, 'urx.Object') && any(strcmp({metaclass(affectedObject).PropertyList.Name}, [affectedPropertyName 'Std']))
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
        % Without disableGetRecursion, it will generate an PreGet event.
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
      elseif affectedObject.ptrType == urx.PtrType.OPTIONAL
        functionCFieldAccessor = [functionCFieldAccessor '_optional'];
      end
      functionCFieldAccessor = [functionCFieldAccessor '_' urx.Object.camelToSnakeCase(affectedPropertyName)];
      libBindingRef = urx.LibBinding.getInstance();
      affectedCFieldPtr = libBindingRef.call(functionCFieldAccessor, affectedObject.id);

      if strcmp(affectedPropertyName, "hwConfig")
        affectedPropertyHwPtr = affectedObject.([affectedPropertyName 'Ptr']);
        if isempty(affectedPropertyHwPtr)
          affectedPropertyHwPtr = uac.HwConfig(affectedCFieldPtr, urx.PtrType.RAW, affectedObject);
          affectedObject.([affectedPropertyName 'Ptr']) = affectedPropertyHwPtr;
        else
          assert(urx.Object.showPtr(affectedPropertyHwPtr.id) == urx.Object.showPtr(affectedCFieldPtr));
        end
      else
        affectedPropertyHwPtr = [];
      end

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
          elseif ~isempty(affectedPropertyStd)
            affectedPropertyStd.id = affectedCFieldPtr;
            affectedPropertyStd.clear();
            % Check consistency.
            if affectedPropertyStd.nbDims == 1
              assert(strcmp(class(affectedProperty), affectedPropertyStd.objectClassName));
              for i = 1:numel(affectedProperty)
                affectedPropertyStd.pushBack(affectedProperty(i));
                % Update id and ptrType
                realAffectedDataI = affectedPropertyStd.data(i);
                if isa(affectedProperty(i), 'urx.Object')
                  affectedProperty(i).id = realAffectedDataI.id;
                  affectedProperty(i).ptrType = realAffectedDataI.ptrType;
                  affectedProperty(i).parent = affectedObject;
                end
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
          elseif ~isempty(affectedPropertyHwPtr)
            affectedPropertyHwPtr.clear();
            affectedPropertyHwPtr.fromStruct(affectedProperty);
          else
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

              % New value has never been affected.
              if isempty(affectedProperty.parent)
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

                % shared stored in weak ptr.
              elseif affectedObject.savePtrType == urx.PtrType.WEAK && affectedProperty.ptrType == urx.PtrType.SHARED
              elseif affectedObject.savePtrType == urx.PtrType.WEAK && affectedProperty.ptrType == urx.PtrType.WEAK
              else
                assert(false);
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
          elseif ~isempty(affectedPropertyStd)
            if urx.Object.showPtr(affectedPropertyStd.id) ~= urx.Object.showPtr(affectedCFieldPtr)
              affectedPropertyStd.id = affectedCFieldPtr;
              len = affectedPropertyStd.len();
              if len == 0
                cppValues = eval([affectedPropertyStd.objectClassName, '.empty']);
              else
                cppValues = repmat(eval([affectedPropertyStd.objectClassName, '([])']), 1, affectedPropertyStd.len());
                for i = 1:numel(cppValues)
                  cppValues(i) = affectedPropertyStd.data(i);
                end
              end
              affectedObject.(affectedPropertyName) = cppValues;
            end
          elseif ~isempty(affectedPropertyHwPtr)
            affectedObject.(affectedPropertyName) = affectedPropertyHwPtr.fromCpp();
          elseif ~isa(affectedProperty, 'urx.Object')
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
          else % urx.Object
            props = metaclass(affectedObject).PropertyList;
            idxProperty = arrayfun(@(x) strcmp(x.Name, affectedPropertyName), props);
            validatorFunc = props(idxProperty).Validation.ValidatorFunctions;

            if any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.sharedPtrInCpp'), validatorFunc))
              stdPtrType = urx.PtrType.SHARED;
            elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.optionalInCpp'), validatorFunc))
              stdPtrType = urx.PtrType.OPTIONAL;
            elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.weakPtrInCpp'), validatorFunc))
              stdPtrType = urx.PtrType.WEAK;
            elseif any(cellfun(@(x) strcmp(func2str(x), 'urx.Validator.rawInCpp'), validatorFunc))
              stdPtrType = urx.PtrType.RAW;
            else
              assert(false);
            end
            if isempty(affectedObject.(affectedPropertyName))
              has_data = libBindingRef.call([functionCFieldAccessor '_has_data'], affectedObject.id);
              % Force value if PreGet is called from a PreSet.
              if (has_data || (disableGetRecursion == 0 && disableSetRecursion == 1 && strcmp(s(2).name, 'Object.handlePropEvents')))
                affectedObject.(affectedPropertyName) = feval(affectedPropertyClassName, affectedCFieldPtr, stdPtrType, affectedObject);
              else
                affectedObject.(affectedPropertyName) = feval(affectedPropertyClassName, []).empty;
              end
            elseif (stdPtrType == urx.PtrType.WEAK && affectedObject.(affectedPropertyName).ptrType == urx.PtrType.SHARED)
              % Type may have changed when assigning WEAK from a SHARED.
              newProperty = feval(affectedPropertyClassName, affectedCFieldPtr, stdPtrType, affectedObject);
              props = properties(newProperty);

              for i = 1:numel(props)
                newProperty.(cell2mat(props(i))) = affectedObject.(affectedPropertyName).(cell2mat(props(i)));
              end

              affectedObject.(affectedPropertyName) = newProperty;
            else
              % If object has already been cached, check if nothing has changed.
              assert(urx.Object.showPtr(affectedObject.(affectedPropertyName).id) == urx.Object.showPtr(affectedCFieldPtr));
              assert(affectedObject.(affectedPropertyName).ptrType == stdPtrType);
            end
          end
          disableSetRecursion = disableSetRecursion - 1;
      end
    end

    % May be useful to ease debug.
    function res = showPtr(ptr)
      res = uint64(urx.LibBinding.getInstance().call('get_pointer', ptr));
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
