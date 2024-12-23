include(GenerateExportHeader)

set(MATLAB_CPP_PRIVATE_SRCS bindings.cpp CMakeLists.txt)
set(MATLAB_HPP_PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/matlab/bindings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/matlab/bindings_decl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/matlab/bindings_impl.h)

add_library(UrxMatlab)
add_library(Urx::UrxMatlab ALIAS UrxMatlab)
target_compile_definitions(UrxMatlab PRIVATE "UrxMatlabBinding_EXPORTS")

target_include_directories(
  UrxMatlab
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
target_link_libraries(
  UrxMatlab
  PUBLIC Urx::Urx Urx::UrxUtils
  PRIVATE Urx::UrxCompilerFlags)
target_sources(
  UrxMatlab
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
         ${MATLAB_HPP_PUBLIC_SRCS}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/urx/matlab/export.h
          ${MATLAB_CPP_PRIVATE_SRCS})

add_library(
  UrxMatlabBinding SHARED
  ${MATLAB_HPP_PUBLIC_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/urx/matlab/export.h
  export_symbol_from_static.cpp)
add_library(Urx::UrxMatlabBinding ALIAS UrxMatlabBinding)

if(UNIX)
  target_link_options(UrxMatlabBinding PRIVATE "-static-libstdc++"
                      "-static-libgcc")
endif(UNIX)

generate_export_header(UrxMatlabBinding BASE_NAME URX_MATLAB EXPORT_FILE_NAME
                       ${CMAKE_CURRENT_BINARY_DIR}/urx/matlab/export.h)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/urx/matlab/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}/urx/matlab/)

target_include_directories(
  UrxMatlabBinding
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
target_link_libraries(UrxMatlabBinding
                      PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,Urx::UrxMatlab>")

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET UrxMatlabBinding
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_RUNTIME_DLLS:UrxMatlabBinding>
      $<TARGET_FILE_DIR:UrxMatlabBinding>
    COMMAND_EXPAND_LISTS)
endif()

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(
  TARGETS UrxMatlabBinding UrxMatlab
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Generate the MATLAB toolbox
configure_file(windows.prj.in windows.prj @ONLY)
file(
  GENERATE
  OUTPUT "$<TARGET_FILE_DIR:UrxMatlabBinding>/windows-$<CONFIG>.prj"
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/windows.prj")
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/windows.prj")

configure_file(linux.prj.in linux.prj @ONLY)
file(
  GENERATE
  OUTPUT "$<TARGET_FILE_DIR:UrxMatlabBinding>/linux-$<CONFIG>.prj"
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/linux.prj")
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/linux.prj")

add_custom_command(
  TARGET UrxMatlabBinding
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../+urx $<TARGET_FILE_DIR:UrxMatlabBinding>/+urx
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/urx
    $<TARGET_FILE_DIR:UrxMatlabBinding>/include/urx
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../../urx
    $<TARGET_FILE_DIR:UrxMatlabBinding>/include/urx
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/urx
    $<TARGET_FILE_DIR:UrxMatlabBinding>/include/urx
  COMMAND_EXPAND_LISTS)
