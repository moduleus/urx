#-----------------------------------------------------------------------------
# Build HDF5
#
#-----------------------------------------------------------------------------

include(ExternalProject)

ExternalProject_add(HDF5
  SOURCE_DIR ${CMAKE_BINARY_DIR}/HDF5
  BINARY_DIR ${CMAKE_BINARY_DIR}/HDF5-build
  URL http://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.5/src/hdf5-1.10.5.tar.gz
  UPDATE_COMMAND ""
  CMAKE_ARGS
    ${ep_common_args}
    -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
    -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF
    -DHDF5_BUILD_CPP_LIB:BOOL=ON
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DHDF5_BUILD_TOOLS:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/HDF5-install
  INSTALL_DIR ${INSTALL_DEPENDENCIES_DIR}
)

if(WIN32)
  set(HDF5_DIR ${CMAKE_BINARY_DIR}/HDF5-install/cmake/hdf5/)
else(WIN32)
  set(HDF5_DIR ${CMAKE_BINARY_DIR}/HDF5-install/share/cmake/hdf5/)
endif(WIN32)

set(HDF5_include_dir ${CMAKE_BINARY_DIR}/HDF5-install/include)
set(HDF5_EXTERNAL_PROJECT "TRUE")
