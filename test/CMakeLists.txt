find_package(Catch2 REQUIRED)

include(Catch)

add_subdirectory(utils)

if(WITH_PYTHON)
  add_subdirectory(python)
endif()

add_subdirectory(urx)

if(WITH_HDF5)
  add_subdirectory(io)
endif()
