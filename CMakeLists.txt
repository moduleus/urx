# CMakeList.txt : CMake project for uff_cpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

project(UffTesting LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) 
 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Enable source groups in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ep_common_args
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
  -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_GENERATOR:STRING=${CMAKE_GENERATOR}
  -DCMAKE_GENERATOR_PLATFORM:STRING=${CMAKE_GENERATOR_PLATFORM}
  -DCMAKE_EXTRA_GENERATOR:STRING=${CMAKE_EXTRA_GENERATOR}
  )

message(STATUS "CMAKE_BUILD_TYPE : "${CMAKE_BUILD_TYPE})
 
# HDF5  
# Maybe use find package before installing a new one 
# if ( find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE}) ) then set(HDF5_DIR <pathToHDF5 already installed>)
if(NOT HDF5_DIR)
  include( ${CMAKE_CURRENT_SOURCE_DIR}/External-HDF5.cmake )
  message(STATUS "Using External Project for HDF5: " ${HDF5_DIR})
else()
  message(STATUS "Using specified HDF5_DIR: ${HDF5_DIR}")
endif()
 
# UFF
add_subdirectory(uff)    
set(UFF_LIBRARY uff) 


# UFF Testing
add_subdirectory(testing) 