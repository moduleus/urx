find_package(Catch2 REQUIRED)

include(Catch)

add_subdirectory(utils)

add_subdirectory(python)

add_subdirectory(urx)

if(WITH_HDF5)
  add_subdirectory(io)
endif()
