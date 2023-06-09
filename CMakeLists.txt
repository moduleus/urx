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

# UFF
add_subdirectory(uff)
add_library(${PROJECT_NAME} STATIC ${UFF_SRCS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(HDF5_EXTERNAL_PROJECT)
add_dependencies(${PROJECT_NAME} HDF5)
endif()

# HDF5  
# Maybe use find package before installing a new one 
if(BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

# With HDF5 option
option(WITH_HDF5 "Build with HDF5" ON)
message(STATUS "${PROJECT_NAME} WITH_HDF5=${WITH_HDF5}")

if(WITH_HDF5)
  # Compile definition
  target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)

  # Link ###
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  if(TARGET hdf5-${SEARCH_TYPE})
    message(STATUS "Found target hdf5-${SEARCH_TYPE}")
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
      message(STATUS "Found HDF5 in ${HDF5_DIR}")
      target_link_libraries(
        ${PROJECT_NAME} PUBLIC hdf5::hdf5-${SEARCH_TYPE}
                               hdf5::hdf5_cpp-${SEARCH_TYPE})
    else()
      # Use external project add
      message(STATUS "Using ExternalProject_add")
      include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/External-HDF5.cmake)
      add_dependencies(${PROJECT_NAME} HDF5)
      target_include_directories(${PROJECT_NAME} PRIVATE ${HDF5_include_dir})
    endif()
  endif()
endif()

set(UFF_LIBRARY Uff) 
# UFF Testing
if(BUILD_TESTING)
  add_subdirectory(testing) 
endif()