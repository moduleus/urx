# Copyright Moduleus 2019

cmake_minimum_required (VERSION 3.9)

project(Uff 
	LANGUAGES C CXX 
	DESCRIPTION "Ultrasound File Format C++ implementation")
	
# Configure compiler ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX d)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Enable source groups in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "${PROJECT_NAME} CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

# HDF5  
# Maybe use find package before installing a new one 
if(BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})

if(NOT HDF5_FOUND AND NOT HDF5_DIR AND NOT TARGET HDF5)
  include( ${CMAKE_CURRENT_SOURCE_DIR}/External-HDF5.cmake )
  message(STATUS "Using External Project for HDF5: " ${HDF5_DIR})
else()
  message(STATUS "Using specified HDF5_DIR: ${HDF5_DIR}")
endif()

# UFF
add_subdirectory(uff)

set(UFF_LIBRARY Uff) 
add_library(${PROJECT_NAME} STATIC ${UFF_SRCS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${UFF_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(HDF5_EXTERNAL_PROJECT)
add_dependencies(${PROJECT_NAME} HDF5)
endif()
set(UFF_LIBRARY Uff) 

# UFF Testing
if(BUILD_TESTING)
  add_subdirectory(testing) 
endif()