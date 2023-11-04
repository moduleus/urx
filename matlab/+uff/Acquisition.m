classdef Acquisition < handle
  properties (Access = public)
    id = libpointer
    groupsFactory(1,1) uff.GroupFactory
  end
  
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    sound_speed(1,1) double
    groups(:,1) = uff.Group.empty(0,1)
  end


  methods 
    function this = Acquisition()
      this.id = calllib('libMatlabCppGlueAcquisition', 'acquisition_new');
      this.groupsFactory = uff.GroupFactory(this);
      mc = metaclass(this);
      props = mc.PropertyList;%properties(this);
      for i = 1:numel(props)
        if props(i).SetObservable
          addlistener(this, props(i).Name, 'PostSet', @uff.Acquisition.handlePropEvents);
        end
        if props(i).GetObservable
          addlistener(this, props(i).Name, 'PreGet', @uff.Acquisition.handlePropEvents);
        end
      end
    end

    function delete(this)
      calllib('libMatlabCppGlueAcquisition', 'acquisition_delete', this.id);
    end
  end

  methods (Static)
    function handlePropEvents(src,evnt)
      s = dbstack;
      if numel(s) > 1 && strcmp(s(2).name, 'Acquisition.handlePropEvents')
        return;
      end
      affectedObj = evnt.AffectedObject;
      affectedObjClass = class(affectedObj);
      name = src.Name;
      functionAccessor = [lower(affectedObjClass(5:end)) '_' name];
      evnt.EventName;
      ptr = calllib('libMatlabCppGlueAcquisition', functionAccessor, affectedObj.id);
      affectedPpt = affectedObj.(name);
      switch evnt.EventName
        case 'PostSet'
          switch class(affectedPpt)
            case 'char'
              calllib('libMatlabCppGlueAcquisition', 'std_string_set', ptr, affectedPpt);
            case 'double'
              ptr.setdatatype('doublePtr', numel(affectedPpt));
              ptr.Value = affectedPpt;
          end
          if strncmp(class(affectedPpt), 'uff.', 4) && ...
                any(strcmp(properties(affectedObj), [name 'Factory']))
            for i = find(cellfun(@(x)isempty(x), {affectedPpt.factoryId}));
              affectedObj.([name 'Factory']).insert(i-1, affectedPpt(i));
            end
          end
        case 'PreGet'
          switch class(affectedPpt)
            case 'char'
              affectedObj.(name) = calllib('libMatlabCppGlueAcquisition', 'std_string_get', ptr);
            case 'double'
              ptr.setdatatype('doublePtr', numel(affectedPpt));
              affectedObj.(name) = ptr.Value;
          end
          if strncmp(class(affectedPpt), 'uff.', 4) && ...
                any(strcmp(properties(affectedObj), [name 'Factory']))
            affectedObj.([name 'Factory']).updateFromCpp();
          end
      end
    end
  end
end
