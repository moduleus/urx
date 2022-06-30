# Copyright Moduleus 2019

cmake_minimum_required(VERSION 3.9)

project(Uff VERSION 0.2.0 LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

### Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX d)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

include(CTest)

if(BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

### Sources files ###

set(UFF_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/uff.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_aperture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_aperture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_channel_data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_channel_data.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_dataset.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_dataset.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_element.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_element.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_element_geometry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_element_geometry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_excitation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_excitation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_impulse_response.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_impulse_response.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_linear_array.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_linear_array.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_matrix_array.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_matrix_array.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_object.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_perimeter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_perimeter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_probe.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_probe.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_rca_array.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_rca_array.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_receive_setup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_receive_setup.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_rotation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_rotation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_timed_event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_timed_event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transform.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transform.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_translation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_translation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_setup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_setup.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_wave.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_wave.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_version.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_version.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_wave.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_wave.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.h)

# Set the tree view in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UFF_SRCS})

find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} branch --remote --points-at HEAD
  OUTPUT_VARIABLE git_branch
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
if ("${git_branch}" MATCHES ".*origin\/main.*")
  set(git_branch ${PROJECT_VERSION})
else()
  string(REPLACE "origin/" "" git_branch "${git_branch}")
  string(REGEX REPLACE "[ \t\r\n]" "" git_branch "${git_branch}")
  string(REGEX REPLACE "[^A-Za-z0-9\.]" "_" git_branch "${git_branch}")
endif()
if (NOT BUILD_SHARED_LIBS)
  set(git_branch "${git_branch}_static")
endif()

### Target library ###
set(UFF_LIBRARY ${PROJECT_NAME})
add_library(${PROJECT_NAME} ${UFF_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${git_branch}")

### Link ###
if (BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
if(TARGET hdf5-${SEARCH_TYPE})
  target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)
  target_link_libraries(${PROJECT_NAME} PUBLIC hdf5-${SEARCH_TYPE} hdf5_cpp-${SEARCH_TYPE})
  target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HDF5_CPP_SRC_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HDF5_SRC_SOURCE_DIR}>)
  target_include_directories(
    ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HDF5_BINARY_DIR}>)
else()
  find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
  if (${HDF5_FOUND})
    target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)
    target_link_libraries(${PROJECT_NAME} PUBLIC hdf5::hdf5-${SEARCH_TYPE} hdf5::hdf5_cpp-${SEARCH_TYPE})
  endif()
endif()
# Treat warnings as errors
target_compile_options(${PROJECT_NAME} PRIVATE
$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Werror -Wall>
$<$<CXX_COMPILER_ID:MSVC>:
     /W3 /WX /EHsc /wd4251>)
target_compile_options(${PROJECT_NAME} PRIVATE
$<$<CXX_COMPILER_ID:GNU>:-Wno-format-truncation>)

### Create cmake config files ###

configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${PROJECT_NAME}-${git_branch})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)

### Install library ###

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch})

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h"
            PATTERN ".git" EXCLUDE
            PATTERN "CI" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch})

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
