# Copyright Moduleus 2019

cmake_minimum_required(VERSION 3.9)

project(Uff VERSION 0.2.0 LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

### Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

#
# HDF5
#
set(LIB_TYPE STATIC) # or SHARED
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
set(HDF5_LIBS ${LINK_LIBS} ${HDF5_CXX_${LIB_TYPE}_LIBRARY})

### Sources files ###

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

if(${HDF5_FOUND})
  set(UFF_SRCS ${UFF_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.h)
endif(${HDF5_FOUND})

# Set the tree view in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UFF_SRCS})

### Target library ###
set(UFF_LIBRARY ${PROJECT_NAME})
if(WIN32)
  add_library(${PROJECT_NAME} STATIC ${UFF_SRCS})
else(WIN32)
  add_library(${PROJECT_NAME} SHARED ${UFF_SRCS})
endif(WIN32)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:>  # <prefix>/include/mylib
)
### Link ###
if(${HDF5_FOUND})
  target_link_libraries (${PROJECT_NAME} ${HDF5_LIBS})
endif(${HDF5_FOUND})
# Treat warnings as errors
target_compile_options(${PROJECT_NAME} PRIVATE
$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Werror -Wall>
$<$<CXX_COMPILER_ID:MSVC>:
     /W3 /WX /EHsc>)

### Create cmake config files ###

configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${PROJECT_NAME})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)

### Install library ###

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

add_subdirectory(Testing)
