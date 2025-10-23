function res = saveToFile(dataset)
  libBinding = urx.LibBinding.getInstance();
  res = libBinding.call('urx_validate_dataset', dataset);
end
