classdef Group < uff.Object % handle
  properties (Access = public)
    % id(1,1) = libpointer
    % container uff.StdVectorGroup {mustBeScalarOrEmpty}
    % containerId int32 {mustBeScalarOrEmpty}
    stdVectorSequence uff.StdVectorEvent {mustBeScalarOrEmpty} = uff.StdVectorEvent.empty(0,1)
  end

  properties (Access = public, SetObservable, GetObservable)
    sampling_type(1,1) uff.Group.SamplingType
    sequence(:,1) = uff.Event.empty(0,1)
    description char
  end

  methods
    function this = Group(varargin)
      this@uff.Object(varargin{:});
      % if nargin < 1
      %   this.id = calllib('libMatlabCppGlueAcquisition', 'group_new');
      % else
      %   this.id = id;
      %   this.containerId = containerId;
      %   this.container = container;
      % end
      this.stdVectorSequence = uff.StdVectorEvent(this);
      % mc = metaclass(this);
      % props = mc.PropertyList;
      % for i = 1:numel(props)
      %   if props(i).SetObservable
      %     addlistener(this, props(i).Name, 'PostSet', @uff.Acquisition.handlePropEvents);
      %   end
      %   if props(i).GetObservable
      %     addlistener(this, props(i).Name, 'PreGet', @uff.Acquisition.handlePropEvents);
      %   end
      % end
    end

    % function deleteCpp(this)
    %   calllib('libMatlabCppGlueAcquisition', 'group_delete', this.id);
    %   this.id = libpointer;
    % end

    % function delete(this)
    %   if isempty(this.containerId)
    %     this.deleteCpp();
    %   end
    % end

    % function res = isAnAllocatedObject(this)
    %   res = isempty(this.container);
    % end
  end
end
