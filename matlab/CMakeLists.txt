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

# Create cpp lib to call it from matlab# Set UFF sources
set(UFF_MATLAB_LIBRARY Uff_Matlab)
set(GROUP_DATA_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/calllib/group_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
add_library(${UFF_MATLAB_LIBRARY} ${GROUP_DATA_SRCS})
add_library(Uff::${UFF_MATLAB_LIBRARY} ALIAS ${UFF_MATLAB_LIBRARY})

# Define include directories
target_include_directories(
  ${UFF_MATLAB_LIBRARY}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${UFF_MATLAB_LIBRARY}-${git_branch}>
)
set_target_properties(${UFF_MATLAB_LIBRARY}
                      PROPERTIES OUTPUT_NAME "${UFF_MATLAB_LIBRARY}")
# Setup compiler flags
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_17)

# add compiler warning flags just when building this project via the
# BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  compiler_flags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/WX;/EHsc;/wd4251>>")
    
target_link_libraries(
  ${UFF_MATLAB_LIBRARY}
  PUBLIC Uff::Uff
  PRIVATE compiler_flags)

  # Create cmake config files ###
  configure_package_config_file(
    ${UFF_MATLAB_LIBRARY}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${UFF_MATLAB_LIBRARY}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${UFF_MATLAB_LIBRARY}-${git_branch})
  
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${UFF_MATLAB_LIBRARY}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)
  
  # Install library ###
  
  install(
    TARGETS ${UFF_MATLAB_LIBRARY} compiler_flags
    EXPORT ${UFF_MATLAB_LIBRARY}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_MATLAB_LIBRARY}-${git_branch})
  
  install(
    EXPORT ${UFF_MATLAB_LIBRARY}Targets
    FILE ${UFF_MATLAB_LIBRARY}Targets.cmake
    NAMESPACE ${UFF_MATLAB_LIBRARY}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_MATLAB_LIBRARY}-${git_branch})
  
  install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_MATLAB_LIBRARY}-${git_branch}
    FILES_MATCHING
    PATTERN "matlab/callib/*.cpp"
    PATTERN ".git" EXCLUDE
    PATTERN "CI" EXCLUDE
    PATTERN "install" EXCLUDE
    PATTERN "build" EXCLUDE
    PATTERN "log.h" EXCLUDE)
  
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${UFF_MATLAB_LIBRARY}Config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/${UFF_MATLAB_LIBRARY}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_MATLAB_LIBRARY}-${git_branch})
  
