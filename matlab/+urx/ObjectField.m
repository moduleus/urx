classdef ObjectField < handle
  properties (Access = protected)
    id(1,1) = libpointer
    libBindingRef = urx.LibBinding.empty(1,0)
    ptrType urx.PtrType
  end
end
