classdef Group < handle
  properties (Access = public)
    id(1,1) = libpointer
    factory uff.GroupFactory {mustBeScalarOrEmpty}
    factoryId int32 {mustBeScalarOrEmpty}
  end

  properties (Access = public, SetObservable, GetObservable)
    % sampling_type
    % sequence
    description char
  end

  methods
    function this = Group(factory, factoryId, id)
      if nargin < 1
        this.id = calllib('libMatlabCppGlueAcquisition', 'group_new');
      else
        this.id = id;
        this.factoryId = factoryId;
        this.factory = factory;
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

    function delete(this)
      if isempty(this.factoryId)
        calllib('libMatlabCppGlueAcquisition', 'group_delete', this.id);
      end
    end
  end
end
