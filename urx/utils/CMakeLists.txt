if(WITH_HDF5)
  # Link ###
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
endif()

# Setup compiler flags
add_library(urx_compiler_flags INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  urx_compiler_flags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/WX;/EHsc;/wd4251>>")

# Set URX sources
set(URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.cpp)
set(URX_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/time_helper.h)

if(WITH_HDF5)
  list(
    APPEND
    URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/io/enums.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.cpp)
  list(
    APPEND
    URX_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/io/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.h)
endif()

# Target library ### Create target
set(URX_UTILS_LIBRARY UrxUtils)
add_library(${URX_UTILS_LIBRARY})
add_library(Urx::${URX_UTILS_LIBRARY} ALIAS ${URX_UTILS_LIBRARY})
target_sources(
  ${URX_UTILS_LIBRARY}
  INTERFACE FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../.. FILES
            ${URX_UTILS_H}
  PRIVATE ${URX_UTILS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
if(ENABLE_PCH)
  target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH})
endif()

set_target_properties(
  ${URX_UTILS_LIBRARY} PROPERTIES OUTPUT_NAME
                                  "${URX_UTILS_LIBRARY}-${git_branch}")

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

target_link_libraries(
  ${URX_UTILS_LIBRARY}
  PUBLIC Urx::Urx
  PRIVATE urx_compiler_flags)
if(WITH_HDF5)
  target_link_libraries(
    ${URX_UTILS_LIBRARY} PRIVATE hdf5::hdf5-${SEARCH_TYPE}
                                 hdf5::hdf5_cpp-${SEARCH_TYPE})
  target_compile_definitions(${URX_UTILS_LIBRARY} PUBLIC URX_WITH_HDF5)
  if(ENABLE_PCH)
    target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH_IO})
  endif()
endif()

# Install library ###

install(
  TARGETS ${URX_UTILS_LIBRARY} urx_compiler_flags
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
