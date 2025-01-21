function mustBeScalarOrEmpty(var)
  if ~(isscalar(var) || isempty(var))
    error('Not scalar nor empty.');
  end
end
