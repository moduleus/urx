# Create matlab mex libraries ###

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(matlab_wrappers)

# Define mexplus library as local third party
find_package(mexplus 0.0.1 REQUIRED)

# Define mex wrapper targets

matlab_add_mex(
  NAME MexGroupData
  SRC Mex/group_data.cpp
  LINK_TO Uff::Uff mexplus::mexplus R2018a)

add_matlab_enum(
  NAME DataType 
  NAMESPACE uff.GroupData
  VALUES INT16 INT32 FLOAT DOUBLE UNDEFINED
  INT_VALUES 0 1 2 3 -1)

# # Install matlab eGal package files along with mex libraries ###

install(TARGETS MexGroupData
        LIBRARY DESTINATION "${MATLAB_INSTALL_LIBDIR}/+uff/private")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/+uff"
        DESTINATION ${MATLAB_INSTALL_LIBDIR})
