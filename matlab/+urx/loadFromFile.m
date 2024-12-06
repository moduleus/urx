function res = loadFromFile(filename)
  dataset_ptr = urx.LibBinding.getInstance().call('urx_load_from_file', filename);
  res = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
end
