classdef GroupFactory < handle
  properties (Access = public)
    id(1,1) = libpointer                              % std_vector base pter
    acquisition uff.Acquisition {mustBeScalarOrEmpty} % parent (enable to hold the pointed memory)
    groups(:,1) uff.Group = uff.Group.empty(0,1)      % matlab group list
  end

  methods
    function this = GroupFactory(acquisition)
      if nargin < 1
        return;
      end
      this.id = calllib('libMatlabCppGlueAcquisition', 'acquisition_groups', acquisition.id);
      this.acquisition = acquisition;
    end

    function insert(this, pos, val)
      this.updateFromCpp();
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_insert', this.id, pos, val.id);
      deleteFctName = ['group' '_delete'];
      calllib('libMatlabCppGlueAcquisition', deleteFctName, val.id);
      this.groups(end+1) = val;
      this.updateFromCpp();
    end

    function erase(this, pos)
      this.updateFromCpp();
      calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_erase', this.id, pos);
      this.updateFromCpp();
    end

    function res = vsize(this) % 'v' for not interfering with matlab size() function
      res = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_size', this.id);
    end

    function updateFromCpp(this)
      groupsBasePtr = calllib('libMatlabCppGlueAcquisition', 'std_vector_uff_group_data', this.id);
      sizeofGroup = calllib('libMatlabCppGlueAcquisition', 'group_sizeof');
      nbGroup = this.vsize();
      groupsBasePtr.setdatatype('uint8Ptr', nbGroup * sizeofGroup);
      for i=1:nbGroup
        if numel(this.groups) < i
          this.groups(i) = uff.Group(this, int32(i - 1), groupsBasePtr + (i-1) * sizeofGroup)
        else
          this.groups(i).id = groupsBasePtr + (i-1) * sizeofGroup;
          this.groups(i).factoryId = int32(i - 1);
          this.groups(i).factory = this;
        end
      end
      if numel(this.groups) > nbGroup
        this.groups(nbGroup:end) = [];
      end
    end
  end
end
