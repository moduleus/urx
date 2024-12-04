classdef Object < urx.ObjectField
  properties (Access = private)
    parent {urx.Validator.mustBeScalarOrEmpty} = urx.Object.empty(1,0)
    saveId
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
      this.libBindingRef = this.getInstance();

      namespace = class(this);
      namespace = namespace(1:3);

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
          this.([props(i).Name 'Std']) = feval([namespace '.StdVector'], class(this.(props(i).Name)), 1, stdPtrType, this);
        end
      end
    end

    function res = getInstance(this)
      res = urx.LibBinding.getInstance();
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

    function res = getRawPtr(this)
      res = this.libBindingRef.call([strrep(class(this), '.', '_') '_raw_ptr' urx.Object.functionPtrType(this.ptrType)], this.id);
    end
  end

  methods (Static)
    function res = functionPtrType(ptrType)
      if ptrType == urx.PtrType.SHARED
        res = '_shared';
      elseif ptrType == urx.PtrType.WEAK
        res = '_weak';
      elseif ptrType == urx.PtrType.OPTIONAL
        res = '_optional';
      else
        res = '_raw';
      end
    end

    function res = functionAssign(className, typeDest, typeSrc)
      res = [className '_assign' urx.Object.functionPtrType(typeDest)];
      if typeSrc
        res = [res '_shared'];
      else
        res = [res '_raw'];
      end
    end

    function res = functionVector(className, func, type, nbDims)
      res = ['vector' urx.Object.functionPtrType(type)];
      if nbDims == 2
        res = [res '_2d'];
      end
      res = [res '_' className '_' func];
    end

    function res = getPtrTypeFromValidator(object, propertyName)
      metaclassObject = metaclass(object);
      props = metaclassObject.PropertyList;
      idxProperty = arrayfun(@(x) strcmp(x.Name, propertyName), props);
      validatorFunc = props(idxProperty).Validation.ValidatorFunctions;
      validatorStr = cellfun(@func2str, validatorFunc, 'UniformOutput', false);

      if any(strcmp(validatorStr, 'urx.Validator.sharedPtrInCpp'))
        res = urx.PtrType.SHARED;
      elseif any(strcmp(validatorStr, 'urx.Validator.optionalInCpp'))
        res = urx.PtrType.OPTIONAL;
      elseif any(strcmp(validatorStr, 'urx.Validator.weakPtrInCpp'))
        res = urx.PtrType.WEAK;
      elseif any(strcmp(validatorStr, 'urx.Validator.rawInCpp'))
        res = urx.PtrType.RAW;
      else
        assert(false);
      end
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
      % if any(arrayfun(@(x) strcmp(x.name, 'datatipinfo'), s)) || any(arrayfun(@(x) strcmp(x.name, 'workspacefunc'), s))
      %   % disp will be shown in tipinfo.
      %   disp(['Disabled ' evnt.EventName ' for ' src.Name '. Shown value may be wrong.'])
      %   return;
      % end

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
      metaclassAffectedObject = metaclass(affectedObject);
      if isa(affectedObject, 'urx.Object') && any(strcmp({metaclassAffectedObject.PropertyList.Name}, [affectedPropertyName 'Std']))
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
      functionCFieldAccessor = [strrep(class(affectedObject), '.', '_')  urx.Object.functionPtrType(affectedObject.ptrType) '_' urx.Object.camelToSnakeCase(affectedPropertyName)];
      libBindingRef = affectedObject.getInstance();
      affectedCFieldPtr = libBindingRef.call(functionCFieldAccessor, affectedObject.id);

      if strcmp(affectedPropertyName, "hwConfig")
        affectedPropertyHwPtr = affectedObject.([affectedPropertyName 'Ptr']);
        if isempty(affectedPropertyHwPtr)
          affectedPropertyHwPtr = uac.HwConfig(affectedCFieldPtr, urx.PtrType.RAW, affectedObject);
          affectedObject.([affectedPropertyName 'Ptr']) = affectedPropertyHwPtr;
        else
          assert(libBindingRef.showPtr(affectedPropertyHwPtr.id) == libBindingRef.showPtr(affectedCFieldPtr));
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
              end
              % Update id and ptrType
              % Do it after all pushBack. std::vector::pushBack may realloc
              % and change all pointer adresses.
              for i = 1:numel(affectedProperty)
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

              namespace = class(affectedObject);
              namespace = namespace(1:3);

              for i = 1:numel(affectedProperty)
                vectori = feval([namespace '.StdVector'], affectedPropertyStd.objectClassName, affectedPropertyStd.nbDims-1, affectedPropertyStd.ptrType);
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
              affectedPropertyPtrType = urx.Object.getPtrTypeFromValidator(affectedObject, affectedPropertyName);

              % empty urx.Object (weak_ptr / optional)
              if isempty(affectedProperty)
                assignFunction = urx.Object.functionAssign(strrep(affectedPropertyClassName, '.', '_'), affectedPropertyPtrType, urx.PtrType.SHARED);
                libBindingRef.call(assignFunction, affectedCFieldPtr, libpointer());
              else
                % When you assign an urx object to another one, you don't
                % assign pointer, you copy data from a pointer to another
                % pointer.
                % So after assigning new data to old data, you need to free new
                % data (that has been copied to old data).
                assignFunction = urx.Object.functionAssign(strrep(affectedPropertyClassName, '.', '_'), affectedPropertyPtrType, affectedProperty.ptrType);
                libBindingRef.call(assignFunction, affectedCFieldPtr, affectedProperty.id);

                % New value has never been affected.
                if isempty(affectedProperty.parent)
                  affectedProperty.freeMem();

                  % Restore pointer and ptrType of the property.
                  affectedProperty.id = affectedObject.saveId;
                  affectedProperty.ptrType = affectedPropertyPtrType;

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
                elseif affectedPropertyPtrType == urx.PtrType.WEAK && affectedProperty.ptrType == urx.PtrType.SHARED
                elseif affectedPropertyPtrType == urx.PtrType.WEAK && affectedProperty.ptrType == urx.PtrType.WEAK
                else
                  assert(false);
                end
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
            affectedPropertyStd.id = affectedCFieldPtr;
            len = affectedPropertyStd.len();
            if len == 0
              cppValues = eval([affectedPropertyStd.objectClassName, '.empty']);
            elseif affectedPropertyStd.nbDims == 1
              cppValues = repmat(eval([affectedPropertyStd.objectClassName, '([])']), 1, len);
              for i = 1:len
                cppValues(i) = affectedPropertyStd.data(i);
              end
            else
              cppValues = cell(1, len);
              for i = 1:len
                vectori = affectedPropertyStd.data(i);
                leni = vectori.len();
                cppValues(i) = {[]};
                for j = 1:leni
                  cppValues{i}(end+1) = vectori.data(j);
                end
              end
            end
            affectedObject.(affectedPropertyName) = cppValues;
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
            affectedPropertyPtrType = urx.Object.getPtrTypeFromValidator(affectedObject, affectedPropertyName);
            if isempty(affectedObject.(affectedPropertyName))
              % Field may be empty (i.e. weak_ptr without data).
              has_data = libBindingRef.call([functionCFieldAccessor '_has_data'], affectedObject.id);
              % Force value if PreGet is called from a PreSet.
              if (has_data || (disableGetRecursion == 0 && disableSetRecursion == 1 && strcmp(s(2).name, 'Object.handlePropEvents')))
                newObject = feval(affectedPropertyClassName, affectedCFieldPtr, affectedPropertyPtrType, affectedObject);
                if isa(affectedProperty, 'urx.RawData')
                  sampling = newObject.samplingType();
                  data = newObject.dataType();
                  realAffectedPropertyClassName = affectedPropertyClassName;
                  if data == 0
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_int16_t'];
                  elseif data == 1
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_int32_t'];
                  elseif data == 2
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_float'];
                  elseif data == 3
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_double'];
                  else
                    assert(false);
                  end
                  if sampling == 0
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_real'];
                  elseif sampling == 1
                    realAffectedPropertyClassName = [realAffectedPropertyClassName '_complex'];
                  else
                    assert(false);
                  end
                  newObject = feval(realAffectedPropertyClassName, affectedCFieldPtr, affectedPropertyPtrType, affectedObject);
                end
              else
                feval_empty = feval(affectedPropertyClassName, []);
                newObject = feval_empty.empty;
              end
              affectedObject.(affectedPropertyName) = newObject;
            elseif (affectedPropertyPtrType == urx.PtrType.WEAK && affectedObject.(affectedPropertyName).ptrType == urx.PtrType.SHARED)
              % Type may have changed when assigning WEAK from a SHARED.
              newProperty = feval(affectedPropertyClassName, affectedCFieldPtr, affectedPropertyPtrType, affectedObject);
              props = properties(newProperty);

              for i = 1:numel(props)
                newProperty.(cell2mat(props(i))) = affectedObject.(affectedPropertyName).(cell2mat(props(i)));
              end

              affectedObject.(affectedPropertyName) = newProperty;
            else
              % If object has already been cached, check if nothing has changed.
              assert(libBindingRef.showPtr(affectedObject.(affectedPropertyName).id) == libBindingRef.showPtr(affectedCFieldPtr));
              assert(affectedObject.(affectedPropertyName).ptrType == affectedPropertyPtrType);
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
