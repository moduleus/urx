function saveToFile(filename, dataset, chunk_group_data, clean_unusable_data, check_data)
  libBinding = urx.LibBinding.getInstance();
  if nargin == 2
    libBinding.call('urx_save_to_file', filename, dataset);
  else
    libBinding.call('urx_save_to_file_options', filename, dataset, chunk_group_data, clean_unusable_data, check_data);
  end
end
