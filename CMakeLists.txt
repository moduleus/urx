cmake_minimum_required(VERSION 3.9)

project(
  Uff
  VERSION 0.6.0
  LANGUAGES C CXX
  DESCRIPTION "Ultrasound File Format C++ implementation")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Configure compiler ###

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LINK_WHAT_YOU_USE ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

include(CTest)

if(BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

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

# setup installer
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)

# Setup compiler flags
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_17)

# # Treat warnings as errors
# target_compile_options(
#   compiler_flags
#   INTERFACE
#     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
#     -Werror
#     -Wall>
#     $<$<CXX_COMPILER_ID:MSVC>:
#     /W3
#     /WX
#     /EHsc
#     /wd4251>)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  compiler_flags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-WX;-EHsc;-wd4251>>")

add_subdirectory("uff")
add_subdirectory("uff_utils")

# Link ###
if(BUILD_SHARED_LIBS)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

# Create cmake config files ###
configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${PROJECT_NAME}-${git_branch})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

file(WRITE "${CMAKE_BINARY_DIR}/version.txt" "${CMAKE_PROJECT_VERSION}")

# if(BUILD_TESTING)
#   enable_testing()
#   add_subdirectory(Testing)
# endif()
