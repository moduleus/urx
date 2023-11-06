classdef Object < handle
  properties (Access = public)
    id(1,1) = libpointer
    container {mustBeScalarOrEmpty} % TODO: ?? uff.StdVectorObject {mustBeScalarOrEmpty}
    containerId int32 {mustBeScalarOrEmpty}
  end

  methods
    function this = Object(container, containerId, id)
      if nargin < 1
        this.id = calllib('libMatlabCppGlueAcquisition', 'group_new');
      else
        this.id = id;
        this.containerId = containerId;
        this.container = container;
      end
      mc = metaclass(this);
      props = mc.PropertyList;
      for i = 1:numel(props)
        if props(i).SetObservable
          addlistener(this, props(i).Name, 'PostSet', @uff.Acquisition.handlePropEvents);
        end
        if props(i).GetObservable
          addlistener(this, props(i).Name, 'PreGet', @uff.Acquisition.handlePropEvents);
        end
      end
    end

    function deleteCpp(this)
      thisClass = class(this);
      calllib('libMatlabCppGlueAcquisition', [lower(thisClass(5:end)) '_delete'], this.id);
      this.id = libpointer;
    end

    function delete(this)
      if isempty(this.containerId)
        this.deleteCpp();
      end
    end

    function res = isAnAllocatedObject(this)
      res = isempty(this.container);
    end
  end
end
