# Create eGal matlab mex libraries ###

# Define mexplus library as local third party
add_library(mexplus INTERFACE)
target_include_directories(mexplus INTERFACE ${PROJECT_SOURCE_DIR}/matlab)

# cmake-format: off
# cmake-format: on

# Define mex wrapper targets

matlab_add_mex(
  NAME MexVector
  SRC Mex/Vector.cpp
  LINK_TO mexplus mexgpu R2020b)

# # Install matlab eGal package files along with mex libraries ###

install(TARGETS MexVector
        LIBRARY DESTINATION "${MATLAB_INSTALL_LIBDIR}/+uff/private")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/+uff"
        DESTINATION ${MATLAB_INSTALL_LIBDIR})
