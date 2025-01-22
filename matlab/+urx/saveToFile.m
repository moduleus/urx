function saveToFile(filename, dataset)
  urx.LibBinding.getInstance().call('urx_save_to_file', filename, dataset.getId());
end
