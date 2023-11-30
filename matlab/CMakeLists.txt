# Create matlab mex libraries ###

# Define mexplus library as local third party
find_package(mexplus 0.0.2 REQUIRED)

# Define mex wrapper targets

matlab_add_mex(
  NAME MexURX
  SRC Mex/acquisition.cpp Mex/dataset.cpp Mex/group_data.cpp Mex/group.cpp
      Mex/load_lib.cpp Mex/version.cpp
  LINK_TO Urx::Urx mexplus::mexplus R2018a)

# # Install matlab eGal package files along with mex libraries ###

install(TARGETS MexURX
        LIBRARY DESTINATION "${MATLAB_INSTALL_LIBDIR}/+urx/private")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/+urx"
        DESTINATION ${MATLAB_INSTALL_LIBDIR})
