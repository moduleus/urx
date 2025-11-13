cmake_minimum_required(VERSION 3.23)

# With options
option(WITH_HDF5 "Build with HDF5" OFF)
if(WITH_HDF5)
  list(APPEND VCPKG_MANIFEST_FEATURES "hdf5")
endif()
option(WITH_MATLAB "Build Matlab wrappers" OFF)
if(WITH_MATLAB)
  list(APPEND VCPKG_MANIFEST_FEATURES "matlab")
endif()
option(WITH_PYTHON "Build Python module" OFF)
if(WITH_PYTHON)
  list(APPEND VCPKG_MANIFEST_FEATURES "python")
endif()
option(WITH_PYTHON_WHL "Generate Python module with pip wheel" OFF)
if(WITH_PYTHON_WHL)
  list(APPEND VCPKG_MANIFEST_FEATURES "python-wheel")
  if(NOT WITH_PYTHON)
    message(
      FATAL_ERROR
        "WITH_PYTHON must be set to true if WITH_PYTHON_WHL is enabled")
  endif()
endif()
option(BUILD_TESTING "Build the testing tree." ON)
if(BUILD_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

# Windows only
option(CRT_SHARED_LIBS "Build using CRT shared libraries" ON)

if(NOT CRT_SHARED_LIBS)
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

project(
  Urx
  VERSION 1.3.1
  LANGUAGES C CXX
  DESCRIPTION "Ultrasound Raw eXchange format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckIPOSupported)

include(cmake/CompilerCaching.cmake)

# Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build" FORCE)
endif()

check_ipo_supported(RESULT result_ipo)
if(result_ipo)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options("$<$<CONFIG:RELEASE>:-ffat-lto-objects>")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN "YES")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LINK_WHAT_YOU_USE ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

option(HARDENED_BUILD "Enable strict flags while building" OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
add_compile_definitions(_SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING)

set(TEST_DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test/data)

include(CTest)
enable_testing()

find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} branch --remote --points-at HEAD
  OUTPUT_VARIABLE git_branch
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  RESULT_VARIABLE GIT_RETVAL)
if("${git_branch}" MATCHES ".*origin\/main.*"
   OR "${git_branch}" MATCHES ".*origin\/release-[0-9].*"
   OR "${git_branch}" STREQUAL ""
   OR NOT GIT_RETVAL EQUAL "0")
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

set(URX_MATLAB_INSTALL_DATADIR
    ${CMAKE_INSTALL_DATAROOTDIR}/${INSTALL_NAME}/matlab)

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

if(NOT CPack_CMake_INCLUDED EQUAL 1)
  include(CPack)
endif()

if(WITH_PYTHON AND (NOT TARGET pybind11::pybind11 OR NOT TARGET Python3::Module
                   ))
  set(PYTHON_IS_DEBUG OFF)
  if(DEFINED ENV{VCPKG_Python3_EXECUTABLE} AND NOT DEFINED Python3_EXECUTABLE)
    set(Python3_EXECUTABLE $ENV{VCPKG_Python3_EXECUTABLE})
  endif()

  find_package(
    Python3
    COMPONENTS Interpreter Development.Module
    REQUIRED)
  find_package(pybind11 2.11 REQUIRED)

  # Check if Python3_EXECUTABLE is debug and modify Python3_EXECUTABLE according
  # to.
  execute_process(
    COMMAND "${Python3_EXECUTABLE}" "-c"
            "import sys; sys.exit(hasattr(sys, 'gettotalrefcount'))"
    RESULT_VARIABLE Python3_EXECUTABLE_IS_DEBUG)

  if(Python3_EXECUTABLE_IS_DEBUG)
    set(Python3_EXECUTABLE_DEBUG ${Python3_EXECUTABLE})
    set(Python3_EXECUTABLE_DEBUG_IS_DEBUG ON)

    # If Python3_EXECUTABLE_IS_DEBUG, it's user's choice.
  else()
    set(Python3_EXECUTABLE_IS_DEBUG OFF)

    set(Python3_EXECUTABLE_DEBUG_IS_DEBUG OFF)

    if(WIN32)
      if(NOT "${Python3_EXECUTABLE}" MATCHES ".*_d\.exe$")
        string(REPLACE ".exe" "_d.exe" Python3_EXECUTABLE_DEBUG_TMP
                       "${Python3_EXECUTABLE}")
      endif()
    else()
      if(NOT "${Python3_EXECUTABLE}" MATCHES ".*d$")
        set(Python3_EXECUTABLE_DEBUG_TMP "${Python3_EXECUTABLE}d")
      endif()
    endif()

    find_program(Python3_EXECUTABLE_DEBUG_NEW_REAL
                 ${Python3_EXECUTABLE_DEBUG_TMP} NO_DEFAULT_PATH)
    if(NOT Python3_EXECUTABLE_DEBUG_NEW_REAL
       AND EXISTS ${Python3_EXECUTABLE_DEBUG_TMP})
      set(Python3_EXECUTABLE_DEBUG_NEW_REAL ${Python3_EXECUTABLE_DEBUG_TMP})
    endif()
    if(Python3_EXECUTABLE_DEBUG_NEW_REAL)
      execute_process(
        COMMAND "${Python3_EXECUTABLE_DEBUG_NEW_REAL}" "-c"
                "import sys; sys.exit(hasattr(sys, 'gettotalrefcount'))"
        RESULT_VARIABLE _Python3_EXECUTABLE_DEBUG_NEW_REAL_IS_DEBUG)
      if(_Python3_EXECUTABLE_DEBUG_NEW_REAL_IS_DEBUG)
        set(Python3_EXECUTABLE_DEBUG_IS_DEBUG ON)
        set(Python3_EXECUTABLE_DEBUG ${Python3_EXECUTABLE_DEBUG_NEW_REAL})
      endif()
    endif()
  endif()

  message(
    STATUS
      "Python3_EXECUTABLE: ${Python3_EXECUTABLE} is debug ? ${Python3_EXECUTABLE_IS_DEBUG}"
  )
  message(
    STATUS
      "Python3_EXECUTABLE_DEBUG: ${Python3_EXECUTABLE_DEBUG} is debug ? ${Python3_EXECUTABLE_DEBUG_IS_DEBUG}"
  )

  set_property(
    TARGET pybind11::pybind11
    APPEND
    PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<CONFIG:Debug>:Py_DEBUG>)
endif()

if(WITH_HDF5)
  option(URX_HDF5_LINK_SHARED "Link shared or static HDF5 lib"
         ${BUILD_SHARED_LIBS})
  if(URX_HDF5_LINK_SHARED)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} HDF5_TYPE)
  # For UrxConfig.cmake.in
  message(STATUS "UrxUtils linking hdf5 ${HDF5_TYPE}")

  if(NOT TARGET hdf5::hdf5-${HDF5_TYPE})
    find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX ${HDF5_TYPE})
  endif()
else()
  set(URX_HDF5_LINK_SHARED ${BUILD_SHARED_LIBS})
endif()

add_subdirectory("urx")

if(WITH_MATLAB)
  add_subdirectory(matlab)
endif()

if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
endif()

if(WITH_PYTHON)
  add_subdirectory(python)
endif()

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${INSTALL_NAME})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# Create cmake config files ###
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

file(WRITE "${CMAKE_BINARY_DIR}/version.txt" "${CMAKE_PROJECT_VERSION}")
