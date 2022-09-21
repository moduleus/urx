# Copyright Moduleus 2019

cmake_minimum_required (VERSION 3.9)

project(UffTesting LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) 

message(STATUS "HDF5_LIBS: ${HDF5_LIBS}")
message(STATUS "HDF5_include_dir: ${HDF5_include_dir}")

# Acquisition 
set(uff_testing uffTestChannelData)
add_executable(${uff_testing} ${uff_testing}.cpp)
target_link_libraries(${uff_testing} PUBLIC ${UFF_LIBRARY})

 