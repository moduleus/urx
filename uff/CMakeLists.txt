# CMakeList.txt : CMake project for uff_cpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

project(uff LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enable source groups in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# HDF5
# Maybe use find package before installing a new one 
# if ( find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE}) ) then set(HDF5_DIR <pathToHDF5 already installed>)
if(NOT HDF5_DIR)
  include( ${CMAKE_CURRENT_SOURCE_DIR}/External-HDF5.cmake )
  message(STATUS "Using External Project for HDF5: " ${HDF5_DIR})
else()
  message(STATUS "Using specified HDF5_DIR: ${HDF5_DIR}")
endif()
 
set(LIB_TYPE STATIC) # or SHARED
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${HDF5_DIR}")
set(HDF5_LIBS ${LINK_LIBS} ${HDF5_CXX_${LIB_TYPE}_LIBRARY})
include_directories(${HDF5_include_dir})

# UFF 
set(UFF_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/uff.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_acquisition.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_acquisition.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_aperture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_aperture.h
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
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group_data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group_data.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group_link.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_group_link.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_igroup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_igroup.h
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
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_sequence.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_sequence.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_super_group.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_super_group.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_timed_event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_timed_event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transform.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transform.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_translation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_translation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_setup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_transmit_setup.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_version.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_version.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_wave.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_wave.h
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/uff_writer.h)
   
set(UFF_LIBRARY uff) 
add_library(${UFF_LIBRARY} STATIC ${UFF_SRCS})
target_include_directories(${UFF_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_dependencies(${UFF_LIBRARY} HDF5)
target_link_libraries (${UFF_LIBRARY} ${HDF5_LIBS})
