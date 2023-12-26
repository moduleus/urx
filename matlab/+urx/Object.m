classdef Object < handle

  properties (Access = public)
    libBindingRef = urx.LibBinding.empty(1,0)
    id(1,1) = libpointer
    saveId
    ownerOfMemory(1,1) logical
    pointerOfShared logical
    saveOwnerOfMemory
  end

  methods
    function this = Object(id, isSharedPointer)
      this.libBindingRef = urx.LibBinding.getInstance();
      this.saveId = containers.Map( 'KeyType', 'char', 'ValueType', 'any');
      this.saveOwnerOfMemory = containers.Map( 'KeyType', 'char', 'ValueType', 'logical');

      if nargin == 0
        this.id = this.libBindingRef.call([strrep(class(this), '.', '_') '_new']);
        this.ownerOfMemory = true;
        this.pointerOfShared = true;
      elseif nargin == 1
        this.id = id;
        this.ownerOfMemory = false;
        this.pointerOfShared = true;
      elseif nargin == 2
        this.id = id;
        this.ownerOfMemory = false;
        this.pointerOfShared = isSharedPointer;
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

    function delete(this)
      if this.ownerOfMemory
        deleteFunction = [strrep(class(this), '.', '_') '_delete'];
        this.libBindingRef.call(deleteFunction, this.id);
        this.id = libpointer;
      end
    end

    function res = className(this)
      thisClass = class(this);
      res = thisClass(5:end); % no urx.
    end
  end

  methods (Static)
    function handlePropEvents(src,evnt)
      persistent disablePostRecursion;
      if isempty(disablePostRecursion)
        disablePostRecursion = 0;
      end
      persistent disableGetRecursion;
      if isempty(disableGetRecursion)
        disableGetRecursion = 0;
      end
      % avoid recursion
      s = dbstack;
      % Security
      if numel(s) == 1
        disablePostRecursion = 0;
        disableGetRecursion = 0;
      end
      % Condition to add a breakpoint if recursion
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents')
        s = dbstack;
      end
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents') && disablePostRecursion > 0 && ...
          (strcmp(evnt.EventName, 'PreSet') || strcmp(evnt.EventName, 'PostSet'))
        return;
      end
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents') && disableGetRecursion > 0 && ...
          strcmp(evnt.EventName, 'PreGet')
        return;
      end
      % Condition to add a breakpoint if recursion allowed
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents')
        s = dbstack;
      end
      % Disable hint for tipinfo and workspace info. To ease debug. Need to
      % be removed before release.
      if any(arrayfun(@(x) strcmp(x.name, 'datatipinfo'), s)) || any(arrayfun(@(x) strcmp(x.name, 'workspacefunc'), s))
        disp('Disabled')
        return;
      end

      % get affected affectedObject/property infos
      affectedObject = evnt.AffectedObject;
      affectedPropertyName = src.Name;
      disableGetRecursion = disableGetRecursion + 1;
      affectedProperty = affectedObject.(affectedPropertyName);
      disableGetRecursion = disableGetRecursion - 1;

      % C pointer of the field.
      functionCFieldAccessor = strrep(class(affectedObject), '.', '_');
      if affectedObject.ownerOfMemory
        functionCFieldAccessor = [functionCFieldAccessor '_shared'];
      end
      functionCFieldAccessor = [functionCFieldAccessor '_' urx.Object.camelToSnakeCase(affectedPropertyName)];
      libBindingRef = urx.LibBinding.getInstance();
      affectedCFieldPtr = libBindingRef.call(functionCFieldAccessor, affectedObject.id);

      affectedPropertyClassName = class(affectedProperty);
      if iscell(affectedPropertyClassName)
        affectedPropertyClassName = class(affectedProperty{1});
      end

      % decision tree: event then, property class (char, double, int32/enum or urx.* from std_vector)
      switch evnt.EventName

        case 'PreSet'
          %if isempty(affectedProperty) && affectedObject.saveId.isKey(affectedPropertyName)
          %  affectedObject.saveId.remove(affectedPropertyName);
          %  affectedObject.saveOwnerOfMemory.remove(affectedPropertyName);
          %end
          if ~isempty(affectedProperty)
            affectedObject.saveId(affectedPropertyName) = affectedProperty.id;
            affectedObject.saveOwnerOfMemory(affectedPropertyName) = affectedProperty.ownerOfMemory;
          else
            affectedObject.saveId(affectedPropertyName) = affectedCFieldPtr;
            affectedObject.saveOwnerOfMemory(affectedPropertyName) = false;
          end

        case 'PostSet'
          if strcmp(affectedPropertyClassName, 'char')
            libBindingRef.call('std_string_set', affectedCFieldPtr, affectedProperty);
          elseif isenum(affectedProperty) && isa(affectedProperty, 'int32')
            if (numel(affectedProperty) ~= 1)
              throw(MException('urx:fatalError', 'Only single value is supported.'));
            end
            affectedCFieldPtr.setdatatype('int32Ptr', numel(affectedProperty));
            affectedCFieldPtr.Value = int32(affectedProperty);
          elseif isa(affectedProperty, 'urx.StdVector')
            % Should be launch only from child constructor after Object
            % constructor has been called.
          elseif ~isa(affectedProperty, 'urx.Object')
            if (numel(affectedProperty) ~= 1)
              throw(MException('urx:fatalError', 'Only single value is supported.'));
            end
            affectedCFieldPtr.setdatatype([affectedPropertyClassName 'Ptr'], numel(affectedProperty));
            affectedCFieldPtr.Value = affectedProperty;
          else % urx affectedObject

            assignFunction = [strrep(affectedPropertyClassName, '.', '_') '_assign'];
            if affectedObject.saveOwnerOfMemory(affectedPropertyName)
              assignFunction = [assignFunction '_shared'];
            else
              if affectedProperty.pointerOfShared && ~affectedProperty.ownerOfMemory
                assignFunction = [assignFunction '_weak'];
              else
                assignFunction = [assignFunction '_raw'];
              end
            end
            if affectedProperty.pointerOfShared
              assignFunction = [assignFunction '_shared'];
            else
              assignFunction = [assignFunction '_raw'];
            end
            libBindingRef.call(assignFunction, affectedObject.saveId(affectedPropertyName), affectedProperty.id);

            if affectedObject.saveId.isKey(affectedPropertyName)
              affectedProperty.id = affectedObject.saveId(affectedPropertyName);
              affectedProperty.ownerOfMemory = affectedObject.saveOwnerOfMemory(affectedPropertyName);
            end
          end

        case 'PreGet'
          disablePostRecursion = disablePostRecursion + 1;
          if strcmp(affectedPropertyClassName, 'char')
            affectedObject.(affectedPropertyName) = libBindingRef.call('std_string_get', affectedCFieldPtr);
          elseif isenum(affectedProperty) && isa(affectedProperty, 'int32')
            if (numel(affectedProperty) ~= 1)
              throw(MException('urx:fatalError', 'Only single value is supported.'));
            end
            affectedCFieldPtr.setdatatype('int32Ptr', numel(affectedProperty));
            affectedCFieldPtr.Value = int32(affectedProperty);
          elseif strcmp(affectedPropertyClassName, 'urx.StdVector')
            affectedObject.(affectedPropertyName).id = affectedCFieldPtr;
          elseif ~isa(affectedProperty, 'urx.Object')
            if (numel(affectedProperty) ~= 1)
              throw(MException('urx:fatalError', 'Only single value is supported.'));
            end

            affectedCFieldPtr.setdatatype([affectedPropertyClassName 'Ptr'], numel(affectedProperty));
            affectedObject.(affectedPropertyName) = affectedCFieldPtr.Value;
          else % urx affectedObject
            affectedObject.(affectedPropertyName) = urx.(affectedPropertyClassName(5:end))(affectedCFieldPtr, false);
          end
          disablePostRecursion = disablePostRecursion - 1;
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
