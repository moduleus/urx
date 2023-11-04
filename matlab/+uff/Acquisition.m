classdef Acquisition < handle
  properties (Access = public)
    id = libpointer
    stdVectorGroup uff.StdVectorGroup {mustBeScalarOrEmpty} = uff.StdVectorGroup.empty(0,1)
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
      this.stdVectorGroup = uff.StdVectorGroup(this);
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
      pptName = src.Name;
      functionAccessor = [lower(affectedObjClass(5:end)) '_' pptName];
      ptr = calllib('libMatlabCppGlueAcquisition', functionAccessor, affectedObj.id);
      affectedPpt = affectedObj.(pptName);
      switch evnt.EventName
        case 'PostSet'
          switch class(affectedPpt)
            case 'char'
              calllib('libMatlabCppGlueAcquisition', 'std_string_set', ptr, affectedPpt);
            case 'double'
              ptr.setdatatype('doublePtr', numel(affectedPpt));
              ptr.Value = affectedPpt;
          end
          if strncmp(class(affectedPpt), 'uff.', 4) && ~strncmp(class(affectedPpt), 'uff.stdV', 9) ...
                && any(strncmp(properties(affectedObj), 'stdVector', 9))
            tiedStdVecName = ['stdVector' upper(pptName(1)) pptName(2:(end-1))];
            tiedStdVecClass = class(affectedObj.(tiedStdVecName));
            tmpStdVec = eval([tiedStdVecClass '()']);
            for i=1:numel(affectedPpt)
              tmpStdVec.pushBack(affectedPpt(i));
              if affectedPpt(i).isAnAllocatedObject()
                affectedPpt(i).deleteCpp();
              end
            end
            tmpStdVec.(pptName) = affectedPpt;
            tmpStdVec.updateFromCpp();
            affectedObj.(tiedStdVecName).copy(tmpStdVec);
            affectedObj.(tiedStdVecName).(pptName) = affectedPpt;
          end
        case 'PreGet'
          switch class(affectedPpt)
            case 'char'
              affectedObj.(pptName) = calllib('libMatlabCppGlueAcquisition', 'std_string_get', ptr);
            case 'double'
              ptr.setdatatype('doublePtr', numel(affectedPpt));
              affectedObj.(pptName) = ptr.Value;
          end
          if strncmp(class(affectedPpt), 'uff.', 4) && ~strncmp(class(affectedPpt), 'uff.stdV', 9) ...
                && any(strncmp(properties(affectedObj), 'stdVector', 9))
            tiedStdVecName = ['stdVector' upper(pptName(1)) pptName(2:(end-1))];
            affectedObj.(tiedStdVecName).updateFromCpp();
            affectedObj.(pptName) = affectedObj.(tiedStdVecName).(pptName);
          end
      end
    end
  end
end
