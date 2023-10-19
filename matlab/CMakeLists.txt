# Create matlab mex libraries ###

# Define mexplus library as local third party
# add_library(mexplus INTERFACE)
# target_include_directories(mexplus INTERFACE ${PROJECT_SOURCE_DIR}/matlab)

find_package(mexplus 0.0.1 REQUIRED)

# Define mex wrapper targets

matlab_add_mex(
  NAME MexGroupData
  SRC Mex/group_data.cpp
  LINK_TO Uff::Uff mexplus::mexplus R2018a)

# # Install matlab eGal package files along with mex libraries ###

install(TARGETS MexGroupData
        LIBRARY DESTINATION "${MATLAB_INSTALL_LIBDIR}/+uff/private")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/+uff"
        DESTINATION ${MATLAB_INSTALL_LIBDIR})
