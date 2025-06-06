function res = loadFromFile(filename)
  libBinding = urx.LibBinding.getInstance();
  dataset_ptr = libBinding.call('urx_load_from_file', filename);
  res = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
end
