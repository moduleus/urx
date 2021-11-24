# CMakeList.txt : CMake project for uff_cpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

project(UffTesting LANGUAGES C CXX DESCRIPTION "Ultrasound File Format C++ implementation")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_subdirectory(uff)  
set(UFF_LIBRARY uff)

add_subdirectory(testing) 