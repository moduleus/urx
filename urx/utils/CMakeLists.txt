if(WITH_HDF5)
  find_file(BOOST_PFR_HPP "boost/pfr.hpp")
  find_package(magic_enum REQUIRED)

  # Link ###
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
endif()

# Setup compiler flags
add_library(compiler_flags INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  compiler_flags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/WX;/EHsc;/wd4251>>")

# Set URX sources
set(URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/time_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

if(WITH_HDF5)
  list(
    APPEND
    URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_v0_3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_v0_3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/upgrade.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/upgrade.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.h)
endif()

# Target library ### Create target
set(URX_UTILS_LIBRARY UrxUtils)
add_library(${URX_UTILS_LIBRARY} ${URX_UTILS_SRCS})
add_library(Urx::${URX_UTILS_LIBRARY} ALIAS ${URX_UTILS_LIBRARY})
if(WITH_PYTHON)
  target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH}
                            ${URX_PCH_PYTHON})
else()
  target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH})
endif()

# Define include directories
target_include_directories(
  ${URX_UTILS_LIBRARY}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${URX_UTILS_LIBRARY}-${git_branch}>
)
set_target_properties(
  ${URX_UTILS_LIBRARY} PROPERTIES OUTPUT_NAME
                                  "${URX_UTILS_LIBRARY}-${git_branch}")

target_link_libraries(
  ${URX_UTILS_LIBRARY}
  PUBLIC Urx::Urx Urx::Urxv0_2
  PRIVATE compiler_flags)
if(WITH_HDF5)
  target_link_libraries(
    ${URX_UTILS_LIBRARY}
    PRIVATE hdf5::hdf5-${SEARCH_TYPE} hdf5::hdf5_cpp-${SEARCH_TYPE}
    PRIVATE magic_enum::magic_enum)
  target_include_directories(${URX_UTILS_LIBRARY} PRIVATE ${Boost_INCLUDE_DIRS})
  target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH_IO})
endif()

# Install library ###

install(
  TARGETS ${URX_UTILS_LIBRARY} compiler_flags
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})
