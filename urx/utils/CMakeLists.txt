include(GenerateExportHeader)

if(WITH_HDF5)
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  if(NOT TARGET hdf5::hdf5-${SEARCH_TYPE})
    find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
  endif()
endif()

add_library(UrxCompilerFlags INTERFACE)
add_library(Urx::UrxCompilerFlags ALIAS UrxCompilerFlags)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  UrxCompilerFlags
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3;/WX;/EHsc;/wd4251>>")

set(URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.cpp)
set(URX_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/test/io.h
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

set(URX_UTILS_LIBRARY UrxUtils)
add_library(${URX_UTILS_LIBRARY})
add_library(Urx::${URX_UTILS_LIBRARY} ALIAS ${URX_UTILS_LIBRARY})

generate_export_header(
  ${URX_UTILS_LIBRARY} BASE_NAME URX_UTILS EXPORT_FILE_NAME
  ${CMAKE_CURRENT_BINARY_DIR}/urx/utils/export.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/urx/utils/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}/urx/utils/)

target_include_directories(
  ${URX_UTILS_LIBRARY}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(
  ${URX_UTILS_LIBRARY} PROPERTIES OUTPUT_NAME "${INSTALL_NAME}" DEBUG_POSTFIX
                                                                "d")

target_sources(
  ${URX_UTILS_LIBRARY}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../.. FILES
         ${URX_UTILS_H}
  PRIVATE ${URX_UTILS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
          ${CMAKE_CURRENT_BINARY_DIR}/urx/utils/export.h)
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
  PRIVATE Urx::UrxCompilerFlags)
if(WITH_HDF5)
  target_link_libraries(
    ${URX_UTILS_LIBRARY} PRIVATE hdf5::hdf5-${SEARCH_TYPE}
                                 hdf5::hdf5_cpp-${SEARCH_TYPE})
  if(ENABLE_PCH)
    target_precompile_headers(${URX_UTILS_LIBRARY} PRIVATE ${URX_PCH_IO})
  endif()
endif()

if(NOT CALL_FROM_SETUP_PY)
  install(
    TARGETS ${URX_UTILS_LIBRARY} UrxCompilerFlags
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
