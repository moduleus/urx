function res = cloneDataset(dataset)
  libBinding = urx.LibBinding.getInstance();
  dataset_ptr = libBinding.call('urx_clone_dataset', dataset);
  res = urx.Dataset(urx.PtrType.SHARED, dataset_ptr);
end
