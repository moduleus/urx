classdef StdVectorGroup < handle
  properties (Access = public)
    id(1,1) = libpointer                              % std_vector base pter
    acquisition uff.Acquisition {mustBeScalarOrEmpty} % parent (enable to hold the pointed memory)
    groups(:,1) uff.Group = uff.Group.empty(0,1)      % matlab group list
  end

  methods
    function this = StdVectorGroup(input)
      if nargin < 1
        this.id = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_new');
        return;
      end
      if isa(input, 'uff.Acquisition')
        this.id = calllib('libMatlabCppGlueAcquisition', 'acquisition_groups', input.id);
        this.acquisition = input;
      end
    end

    function delete(this)
      if this.isAnAllocatedObject()
        calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_delete', this.id);
      end
    end

    function res = isAnAllocatedObject(this)
      res = isempty(this.acquisition);
    end

    function clear(this)
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_clear', this.id);
    end

    function pushBack(this, val)
      assert(isa(val, 'uff.Group'))
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_push_back', this.id, val.id);
    end

    function res = len(this) % 'len' instead of 'size' for not interfering with matlab size() function
      res = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_size', this.id);
    end

    function updateFromCpp(this)
      groupsBasePtr = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_data', this.id);
      sizeofGroup = calllib('libMatlabCppGlueAcquisition', 'group_sizeof');
      nbGroup = this.len();
      groupsBasePtr.setdatatype('uint8Ptr', nbGroup * sizeofGroup);
      for i=1:nbGroup
        if numel(this.groups) < i
          this.groups(i) = uff.Group(this, int32(i - 1), groupsBasePtr + (i-1) * sizeofGroup)
        else
          this.groups(i).id = groupsBasePtr + (i-1) * sizeofGroup;
          this.groups(i).containerId = int32(i - 1);
          this.groups(i).container = this;
        end
      end
      if numel(this.groups) > nbGroup
        this.groups(nbGroup:end) = [];
      end
    end

    function copy(this, other)
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_copy', this.id, other.id);
    end
  end
end
