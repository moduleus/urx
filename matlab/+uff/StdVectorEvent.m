classdef StdVectorEvent < handle
  properties (Access = public)
    id(1,1) = libpointer                              % std_vector base pter
    group uff.Group {mustBeScalarOrEmpty} % parent (enable to hold the pointed memory)
    sequence(:,1) uff.Event = uff.Event.empty(0,1)      % matlab event list
  end

  methods
    function this = StdVectorEvent(input)
      if nargin < 1
        this.id = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_new');
        return;
      end
      if isa(input, 'uff.Group')
        this.id = calllib('libMatlabCppGlueAcquisition', 'group_sequence', input.id);
        this.group = input;
      end
    end

    function delete(this)
      this.clear();
      if this.isAnAllocatedObject()
        calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_delete', this.id);
      end
    end

    function updateContainer(this, group)
      this.id = calllib('libMatlabCppGlueAcquisition', 'group_sequence', group.id);
      this.group = group;
    end

    function res = isAnAllocatedObject(this)
      res = isempty(this.group);
    end

    function clear(this)
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_clear', this.id);
    end

    function pushBack(this, val)
      assert(isa(val, 'uff.Event'))
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_push_back', this.id, val.id);
    end

    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_size', this.id);
    end

    function updateFromCpp(this)
      sequenceBasePtr = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_data', this.id);
      sizeofEvent = calllib('libMatlabCppGlueAcquisition', 'event_sizeof');
      nbEvent = this.len();
      sequenceBasePtr.setdatatype('uint8Ptr', nbEvent * sizeofEvent);
      for i=1:nbEvent
        if numel(this.sequence) < i
          this.sequence(i) = uff.Event(this, int32(i - 1), sequenceBasePtr + (i-1) * sizeofEvent)
        else
          this.sequence(i).id = sequenceBasePtr + (i-1) * sizeofEvent;
          this.sequence(i).containerId = int32(i - 1);
          this.sequence(i).container = this;
          sequencePpt = properties(this.sequence);
          sequencePptStdVecInd = find(strncmp(sequencePpt, 'stdVector', 9));
          for i=1:numel(sequencePptStdVecInd)
            for j=1:numel(this.sequence)
              this.sequence(j).(sequencePpt{sequencePptStdVecInd(i)}).updateContainer(this.sequence(i));
            end
          end
        end
      end
      if numel(this.sequence) > nbEvent
        this.sequence(nbEvent:end) = [];
      end
    end

    function copy(this, other)
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_event_copy', this.id, other.id);
    end
  end
end
