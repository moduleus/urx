cmake_minimum_required(VERSION 3.9)

project(
  Urx
  VERSION 0.6.11
  LANGUAGES C CXX
  DESCRIPTION "Ultrasound Raw eXchange format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LINK_WHAT_YOU_USE ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(ENABLE_PCH "Build with PCH" ON)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
add_compile_definitions(_SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING)

include(CTest)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} branch --remote --points-at HEAD
  OUTPUT_VARIABLE git_branch
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
if("${git_branch}" MATCHES ".*origin\/main.*" OR "${git_branch}" STREQUAL "")
  set(git_branch ${PROJECT_VERSION})
else()
  string(REPLACE "origin/" "" git_branch "${git_branch}")
  string(REGEX REPLACE "[ \t\r\n]" "" git_branch "${git_branch}")
  string(REGEX REPLACE "[^A-Za-z0-9\.]" "_" git_branch "${git_branch}")
endif()
if(NOT BUILD_SHARED_LIBS)
  set(git_branch "${git_branch}_static")
endif()

set(INSTALL_NAME "${PROJECT_NAME}-${git_branch}")

set(MATLAB_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}/${INSTALL_NAME}/matlab/)

if(CALL_FROM_SETUP_PY)
  set(PYTHON_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX})
else()
  set(PYTHON_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}/${INSTALL_NAME}/python)
endif()

# setup installer
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)

# With options
option(WITH_HDF5 "Build with HDF5" OFF)
option(WITH_MATLAB "Build Matlab wrappers" OFF)
option(WITH_PYTHON "Build Python module" OFF)
enable_testing()

if(ENABLE_PCH)
  set(URX_PCH
      "<algorithm>"
      "<array>"
      "<cassert>"
      "<cmath>"
      "<complex>"
      "<cstddef>"
      "<cstdint>"
      "<cstdio>"
      "<cstdlib>"
      "<cstring>"
      "<exception>"
      "<iomanip>"
      "<ios>"
      "<iosfwd>"
      "<iostream>"
      "<istream>"
      "<iterator>"
      "<limits>"
      "<locale>"
      "<memory>"
      "<optional>"
      "<ostream>"
      "<regex>"
      "<sstream>"
      "<stdexcept>"
      "<string_view>"
      "<string>"
      "<type_traits>"
      "<typeindex>"
      "<unordered_map>"
      "<utility>"
      "<variant>"
      "<vector>"
      "<version>")
  if(WITH_HDF5)
    set(URX_PCH_IO_V2_0 "<H5Cpp.h>")
    set(URX_PCH_IO "<H5Cpp.h>" "<boost/pfr.hpp>" "<magic_enum.hpp>")
  endif()
  if(BUILD_TESTING)
    set(URX_PCH_TEST
        "<catch2/catch_test_macros.hpp>"
        "<catch2/matchers/catch_matchers_floating_point.hpp>"
        "<catch2/matchers/catch_matchers.hpp>")
  endif()
  set(URX_PCH_PYTHON
      "<pybind11/attr.h>"
      "<pybind11/buffer_info.h>"
      "<pybind11/cast.h>"
      "<pybind11/numpy.h>"
      "<pybind11/operators.h>"
      "<pybind11/pybind11.h>"
      "<pybind11/pytypes.h>"
      "<pybind11/stl_bind.h>")
endif()

add_subdirectory("urx")

if(WITH_PYTHON)
  find_package(
    Python3
    COMPONENTS Interpreter Development
    REQUIRED)
  find_package(pybind11 CONFIG REQUIRED)
endif()

if(WITH_MATLAB)
  add_subdirectory(matlab)
endif()

if(WITH_PYTHON)
  add_subdirectory(python)
endif()

if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
  add_subdirectory(test)
endif()

# Create cmake config files ###
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

file(WRITE "${CMAKE_BINARY_DIR}/version.txt" "${CMAKE_PROJECT_VERSION}")
