if(WITH_HDF5)
  find_file(BOOST_PFR_HPP "boost/pfr.hpp")
  find_package(magic_enum CONFIG REQUIRED)

  # Link ###
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
endif()

# Setup compiler flags
add_library(compiler_flags INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  compiler_flags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/WX;/EHsc;/wd4251>>")

# Set UFF sources
set(UFF_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/io.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

if(WITH_HDF5)
  list(
    APPEND
    UFF_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.hpp)
endif()

# Target library ### Create target
set(UFF_UTILS_LIBRARY Uff_Utils)
add_library(${UFF_UTILS_LIBRARY} ${UFF_UTILS_SRCS})
add_library(Uff::${UFF_UTILS_LIBRARY} ALIAS ${UFF_UTILS_LIBRARY})

# Define include directories
target_include_directories(
  ${UFF_UTILS_LIBRARY}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch}>
)
set_target_properties(
  ${UFF_UTILS_LIBRARY} PROPERTIES OUTPUT_NAME
                                  "${UFF_UTILS_LIBRARY}-${git_branch}")

target_link_libraries(
  ${UFF_UTILS_LIBRARY}
  PUBLIC Uff::Uff
  PRIVATE compiler_flags)
if(WITH_HDF5)
  target_link_libraries(
    ${UFF_UTILS_LIBRARY}
    PUBLIC hdf5::hdf5-${SEARCH_TYPE} hdf5::hdf5_cpp-${SEARCH_TYPE}
    PRIVATE magic_enum::magic_enum)
endif()

# Create cmake config files ###
configure_package_config_file(
  ${UFF_UTILS_LIBRARY}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${UFF_UTILS_LIBRARY}-${git_branch})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

# Install library ###

install(
  TARGETS ${UFF_UTILS_LIBRARY} compiler_flags
  EXPORT ${UFF_UTILS_LIBRARY}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch})

install(
  EXPORT ${UFF_UTILS_LIBRARY}Targets
  FILE ${UFF_UTILS_LIBRARY}Targets.cmake
  NAMESPACE ${UFF_UTILS_LIBRARY}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_UTILS_LIBRARY}-${git_branch})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch}
  FILES_MATCHING
  PATTERN "uff_utils/*.hpp"
  PATTERN "uff_utils/*.h"
  PATTERN ".git" EXCLUDE
  PATTERN "CI" EXCLUDE
  PATTERN "install" EXCLUDE
  PATTERN "build" EXCLUDE
  PATTERN "log.h" EXCLUDE)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_UTILS_LIBRARY}-${git_branch})
