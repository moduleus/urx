function res = loadFromFile(filename, raw_data_load_policy)
  libBinding = urx.LibBinding.getInstance();
  if nargin == 1
    dataset_ptr = libBinding.call('urx_load_from_file', filename);
  else
    dataset_ptr = libBinding.call('urx_load_from_file_options', filename, raw_data_load_policy);
  end
  res = urx.Dataset(urx.PtrType.SHARED, dataset_ptr);
end
