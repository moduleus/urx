classdef Vector3D < urx.Object
  properties (Access = public, SetObservable, GetObservable)
    x(1,1)
    y(1,1)
    z(1,1)
  end

  methods
    function this = Vector3D(varargin)
      if (numel(varargin) == 3 && isnumeric(varargin{1}) && ...
          isnumeric(varargin{2}) && isnumeric(varargin{3}))
        args = {};
      else
        args = varargin;
      end
      this@urx.Object(args{:});
      if (numel(varargin) == 3 && isnumeric(varargin{1}) && ...
          isnumeric(varargin{2}) && isnumeric(varargin{3}))
        this.x = varargin{1};
        this.y = varargin{2};
        this.z = varargin{3};
      end
    end
  end
end
