# Copyright Moduleus 2019

cmake_minimum_required(VERSION 3.9)

project(
  Uff
  VERSION 0.4.2
  LANGUAGES C CXX
  DESCRIPTION "Ultrasound File Format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

include(CTest)

if(BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Sources files ###
add_subdirectory("uff")

# Set the tree view in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UFF_SRCS})

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

# Target library ###
set(UFF_LIBRARY ${PROJECT_NAME})
add_library(${PROJECT_NAME} ${UFF_SRCS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
if(ANDROID)
  target_link_libraries(${PROJECT_NAME} PUBLIC log)
endif()
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}>
)
set_target_properties(${PROJECT_NAME}
                      PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${git_branch}")

# Link ###
if(BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
if(TARGET hdf5-${SEARCH_TYPE})
  target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)
  target_link_libraries(${PROJECT_NAME} PUBLIC hdf5-${SEARCH_TYPE}
                                               hdf5_cpp-${SEARCH_TYPE})
  target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HDF5_CPP_SRC_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_include_directories(${PROJECT_NAME}
                             PUBLIC $<BUILD_INTERFACE:${HDF5_SRC_SOURCE_DIR}>)
  target_include_directories(${PROJECT_NAME}
                             PUBLIC $<BUILD_INTERFACE:${HDF5_BINARY_DIR}>)
else()
  find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
  if(${HDF5_FOUND})
    target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)
    target_link_libraries(${PROJECT_NAME} PUBLIC hdf5::hdf5-${SEARCH_TYPE}
                                                 hdf5::hdf5_cpp-${SEARCH_TYPE})
  endif()
endif()
# Treat warnings as errors
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Werror
    -Wall>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W3
    /WX
    /EHsc
    /wd4251>)

# Create cmake config files ###

configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${PROJECT_NAME}-${git_branch})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

# Install library ###

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch})

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h"
  PATTERN ".git" EXCLUDE
  PATTERN "CI" EXCLUDE
  PATTERN "log.h" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch})

file(WRITE "${CMAKE_BINARY_DIR}/version.txt" "${CMAKE_PROJECT_VERSION}")

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
