classdef ObjectField < handle
  properties (Access = {?urx.LibBinding, ?urx.Object, ?urx.StdVector})
    id(1,1) = libpointer
  end

  properties (Access = protected)
    libBindingRef = urx.LibBinding.empty(1,0)
    ptrType urx.PtrType
  end
end
