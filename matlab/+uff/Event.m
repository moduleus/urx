classdef Event < uff.Object % handle
  % properties (Access = public)
  %   id(1,1) = libpointer
  %   container uff.StdVectorEvent {mustBeScalarOrEmpty}
  %   containerId int32 {mustBeScalarOrEmpty}
  % end

  properties (Access = public, SetObservable, GetObservable)
    % transmit_setup
    % receive_setup
  end

  methods
    function this = Event(varargin)
      this@uff.Object(varargin{:});
      % if nargin < 1
      %   this.id = calllib('libMatlabCppGlueAcquisition', 'event_new');
      % else
      %   this.id = id;
      %   this.containerId = containerId;
      %   this.container = container;
      % end
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
    %   calllib('libMatlabCppGlueAcquisition', 'event_delete', this.id);
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
