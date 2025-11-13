include(GenerateExportHeader)

if(WITH_HDF5)
  if(NOT TARGET hdf5::hdf5-${HDF5_TYPE})
    find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX ${HDF5_TYPE})
  endif()
endif()

add_library(UrxCompilerFlags INTERFACE)
add_library(Urx::UrxCompilerFlags ALIAS UrxCompilerFlags)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
if(HARDENED_BUILD)
  target_compile_options(
    UrxCompilerFlags
    INTERFACE
      "$<${gcc_like_cxx}:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
      "$<${msvc_cxx}:/W3;/WX;/wd4251>")
endif()

set(URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/clone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raw_data_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serialize_helper.cpp)
set(URX_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/clone.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clone.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/exception.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data_reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/test/dataset_gen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/raw_data_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/serialize_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/validator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/time_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/type_container.h)

if(WITH_HDF5)
  list(
    APPEND
    URX_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/io/enums.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.cpp)
  list(
    APPEND
    URX_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/io/detail/hdf5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_options.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/serialize_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/stream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer_options.h
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
  ${URX_UTILS_LIBRARY}
  PROPERTIES OUTPUT_NAME "${URX_UTILS_LIBRARY}-${git_branch}" DEBUG_POSTFIX "d")

target_sources(
  ${URX_UTILS_LIBRARY}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../.. FILES
         ${URX_UTILS_H}
  PRIVATE ${URX_UTILS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
          ${CMAKE_CURRENT_BINARY_DIR}/urx/utils/export.h)

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

target_link_libraries(
  ${URX_UTILS_LIBRARY}
  PUBLIC Urx::Urx
  PRIVATE Urx::UrxCompilerFlags)
if(WITH_HDF5)
  target_compile_definitions(${URX_UTILS_LIBRARY} PRIVATE WITH_MATLAB)
  target_link_libraries(
    ${URX_UTILS_LIBRARY} PRIVATE hdf5::hdf5-${HDF5_TYPE}
                                 hdf5::hdf5_cpp-${HDF5_TYPE})
endif()

if(MSVC)
  target_compile_options(${URX_UTILS_LIBRARY} PRIVATE /bigobj)
endif()

if(NOT CALL_FROM_SETUP_PY)
  install(
    TARGETS ${URX_UTILS_LIBRARY} UrxCompilerFlags
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
