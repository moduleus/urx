function saveToFile(filename, dataset)
  libBinding = urx.LibBinding.getInstance();
  libBinding.call('urx_save_to_file', filename, dataset);
end
