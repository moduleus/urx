# Copyright Moduleus 2019

message(STATUS "HDF5_LIBS"${HDF5_LIBS})
message(STATUS "HDF5_include_dir : "${HDF5_include_dir})

# Acquisition 
set(uff_testing uffTestChannelData)
add_executable(${uff_testing} ${uff_testing}.cpp)
#add_dependencies(${uff_testing} HDF5)
target_link_libraries(${uff_testing} PUBLIC ${UFF_LIBRARY})
#target_link_libraries (${uff_testing} ${HDF5_LIBS})

#add_test(${uff_testing} ${uff_testing})

 