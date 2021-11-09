# Copyright Moduleus 2019

cmake_minimum_required(VERSION 3.9)

project(Uff LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")
#project(Uff VERSION 0.1.0 )

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
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_reader.h
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
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

set(UFF_LIBRARY Uff)
add_library(${UFF_LIBRARY} STATIC ${UFF_SRCS})
target_include_directories(${UFF_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# HDF5
#
set(LIB_TYPE STATIC) # or SHARED
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}")
set(HDF5_LIBS ${LINK_LIBS} ${HDF5_CXX_${LIB_TYPE}_LIBRARY})
target_link_libraries (${UFF_LIBRARY} ${HDF5_LIBS})

add_subdirectory(Testing)
