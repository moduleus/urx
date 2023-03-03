# Copyright Moduleus 2019

# ChannelData
add_executable(uffTestChannelData uffTestChannelData.cpp)
target_link_libraries(uffTestChannelData PRIVATE ${UFF_LIBRARY})
add_test(uffTestChannelData uffTestChannelData)

if(${HDF5_FOUND})
  add_subdirectory(hdf5)
endif()
