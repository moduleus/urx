classdef Object < handle
  properties (Access = public)
    libBindingRef = urx.LibBinding.empty(1,0)
    id(1,1) = libpointer
    container {mustBeScalarOrEmpty}
    containerId int32 {mustBeScalarOrEmpty}
  end
  
  % this.className() return the lower className, not "Object".
  
  methods
    function this = Object(container, containerId, id)
      this.libBindingRef = urx.LibBinding.getInstance();
      if nargin == 0
        this.id = this.libBindingRef.call([strrep(class(this), '.', '_') '_new']);
      elseif nargin == 1
        this.id = container;
      else
        this.id = id;
        this.containerId = containerId;
        this.container = container;
      end
      mc = metaclass(this);
      props = mc.PropertyList;
      for i = 1:numel(props)
        if strncmp(props(i).Name, 'stdVector', 9)
          tiedObjName = (this.camelToSnakeCase(props(i).Name(10:end)));
          
          prop_id = find(strcmpi(tiedObjName, {props.Name}));
          if (isempty(prop_id))
            throw(MException('urx:fatalError', ['Failed to found ' tiedObjName ' property for ' this.className() '.stdVector' tiedObjName]));
          end
          
          defaultVal = props(prop_id).DefaultValue;
          tiedObjClass = defaultVal.className();
          
          if strcmp(tiedObjClass, 'cell')
            tiedObjClass = class(defaultVal{1});
          end
          propNameSnake = this.camelToSnakeCase(props(i).Name);
          this.(props(i).Name) = urx.StdVector(tiedObjClass, this, propNameSnake(12:end), ...
            isa(this.(tiedObjName), 'cell') + 1);
        end
        if props(i).SetObservable
          addlistener(this, props(i).Name, 'PostSet', @urx.Object.handlePropEvents);
        end
        if props(i).GetObservable
          addlistener(this, props(i).Name, 'PreGet', @urx.Object.handlePropEvents);
        end
      end
    end
    
    function deleteCpp(this)
      deleteFunction = [strrep(class(this), '.', '_') '_delete'];
      this.libBindingRef.call(deleteFunction, this.id);
      this.id = libpointer;
    end
    
    function delete(this)
      if isempty(this.containerId)
        this.deleteCpp();
      end
    end
    
    function res = className(this)
      thisClass = class(this);
      res = thisClass(5:end); % no urx.
    end
    
    function res = isAnAllocatedObject(this)
      res = isempty(this.container);
    end
  end
  
  methods (Static)
    function handlePropEvents(src,evnt)
      % avoid recursion
      s = dbstack;
      if numel(s) > 1 && strcmp(s(2).name, 'Object.handlePropEvents')
        return;
      end
      % While debugging.
      
      if numel(s) > 1 && (strcmp(s(2).name, 'datatipinfo') || strcmp(s(2).name, 'workspacefunc'))
        return;
      end
      
      % get affected object/property infos
      affectedObj = evnt.AffectedObject;
      affectedPptName = src.Name;
      
      % potentially
      tiedStdVecName = ['stdVector' urx.Object.snakeToCamelCase(affectedPptName)];
      
      % C pointer of the field.
      functionAccessor = [strrep(class(affectedObj), '.', '_') '_' affectedPptName];
      libBindingRef = urx.LibBinding.getInstance();
      ptr = libBindingRef.call(functionAccessor, affectedObj.id);
      
      affectedPpt = affectedObj.(affectedPptName);
      affectedPptClass = class(affectedPpt);
      if iscell(affectedPptClass)
        affectedPptClass = class(affectedPpt{1});
      end
      
      % decision tree: event then, property class (char, double, int32/enum or urx.* from std_vector)
      switch evnt.EventName
        
        case 'PostSet'
          if strcmp(affectedPptClass, 'char')
            libBindingRef.call('std_string_set', ptr, affectedPpt);
          elseif ismember(affectedPptClass, {'double', 'uint16', 'int32'})
            affectedObjPpts = properties(affectedObj);
            
            % Check if std::vector
            if any(strcmp(affectedObjPpts, tiedStdVecName))
              affectedObj.(tiedStdVecName).setToCpp(affectedObj.(affectedPptName));
            else
              if (numel(affectedPpt) ~= 1)
                throw(MException('urx:fatalError', 'Only single value is supported.'));
              end
              ptr.setdatatype([affectedPptClass 'Ptr'], numel(affectedPpt));
              ptr.Value = affectedPpt;
            end
          else
            throw(MException('urx:fatalError', [affectedPptClass ' is not supported in PostSet']));
          end
          
          if strncmp(affectedPptClass, 'urx.', 4) && ~strncmp(affectedPptClass, 'urx.stdV', 8)
            if any(strcmp(properties(affectedObj), tiedStdVecName)) % pool of instances case
              tiedStdVecObjectClass = affectedObj.(tiedStdVecName).objectClassName;
              tmpStdVec = urx.StdVector(tiedStdVecObjectClass); % temporary std vector object
              for i=1:numel(affectedPpt)
                tmpStdVec.pushBack(affectedPpt(i));     % push all right value to the tmp object
                if affectedPpt(i).isAnAllocatedObject()
                  affectedPpt(i).deleteCpp();           % free memory of dynamic/stand-alone urx.object
                end
              end
              tmpStdVec.objects = affectedPpt;        % assign same right value to the std vector
              tmpStdVec.updateFromCpp();              % from cpp update (recursive std vector)
              affectedObj.(tiedStdVecName).copy(tmpStdVec); % final copy (cpp side)
              affectedObj.(tiedStdVecName).objects = affectedPpt; % matlab side update
            elseif ~isempty(affectedPpt) % simple pointer case
              tiedStdVector = unique([affectedPpt.container]);
              assert(numel(tiedStdVector) == 1, 'numel(tiedStdVector) should be == 1')
              tiedStdVector.updateFromCpp();
            end
          end
          
        case 'PreGet'
          if strncmp(affectedPptClass, 'urx.', 4)
            affectedObj.(affectedPptName) = urx.(affectedPptClass(5:end))(ptr);
          elseif strcmp(affectedPptClass, 'char')
            affectedObj.(affectedPptName) = libBindingRef.call('std_string_get', ptr);
          elseif strcmp(affectedPptClass, 'double')
            affectedObjPpts = properties(affectedObj);
            if any(strcmp(affectedObjPpts, tiedStdVecName))
              affectedObj.(affectedPptName) = affectedObj.(tiedStdVecName).getFromCpp();
            else
              if (numel(affectedPpt) ~= 1)
                throw(MException('urx:fatalError', 'Only single value is supported.'));
              end
              
              ptr.setdatatype('doublePtr', numel(affectedPpt));
              affectedObj.(affectedPptName) = ptr.Value;
            end
          else
            throw(MException('urx:fatalError', [affectedPptClass ' is not supported in PreGet']));
          end
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
