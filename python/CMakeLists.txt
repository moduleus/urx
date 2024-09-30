include(GenerateExportHeader)

set(URX_PYTHON_LIBRARY UrxPython)
set(URX_PYTHON_LIBRARY_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_library.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_el_geo_imp_resp_exc_probe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_float.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_group_ele_evt_group_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_int16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_int32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_int64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_int8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_vector_string_urx_vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/python/utils/group_helper.cpp)
set(URX_PYTHON_LIBRARY_H
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/python/bindings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx/python/utils/group_helper.h)

add_library(${URX_PYTHON_LIBRARY})
add_library(Urx::${URX_PYTHON_LIBRARY} ALIAS ${URX_PYTHON_LIBRARY})

generate_export_header(
  ${URX_PYTHON_LIBRARY} BASE_NAME URX_PYTHON EXPORT_FILE_NAME
  ${CMAKE_CURRENT_BINARY_DIR}/urx/python/export.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/urx/python/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}/urx/python)

target_link_libraries(
  ${URX_PYTHON_LIBRARY}
  PUBLIC Python3::Module pybind11::pybind11 Urx::UrxUtils
  PRIVATE Urx::UrxCompilerFlags)
if(MSVC)
  target_compile_options(${URX_PYTHON_LIBRARY} PRIVATE /bigobj)
endif()
target_sources(
  ${URX_PYTHON_LIBRARY}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
         ${URX_PYTHON_LIBRARY_H}
  PRIVATE ${URX_PYTHON_LIBRARY_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
          ${CMAKE_CURRENT_BINARY_DIR}/urx/python/export.h)
if(ENABLE_PCH)
  target_precompile_headers(${URX_PYTHON_LIBRARY} PRIVATE ${URX_PCH}
                            ${URX_PCH_PYTHON})
endif()
target_include_directories(
  ${URX_PYTHON_LIBRARY}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(
  ${URX_PYTHON_LIBRARY} PROPERTIES OUTPUT_NAME
                                   "${URX_PYTHON_LIBRARY}-${git_branch}")

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

# Build python binary distribuable.
if(WITH_PYTHON_WHL)
  # Generate pyurx-0.6.26-cp311-cp311-win_amd64.whl
  set(PYTHON_MODULE_PYURX_OUTPUT
      "pyurx-${CMAKE_PROJECT_VERSION}-cp${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}-cp${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}$<$<CONFIG:Debug>:d>"
  )
  if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PYTHON_MODULE_PYURX_OUTPUT
          "${PYTHON_MODULE_PYURX_OUTPUT}-win_amd64.whl")
    else()
      set(PYTHON_MODULE_PYURX_OUTPUT "${PYTHON_MODULE_PYURX_OUTPUT}-win32.whl")
    endif()
  elseif(UNIX AND NOT APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PYTHON_MODULE_PYURX_OUTPUT
          "${PYTHON_MODULE_PYURX_OUTPUT}-linux_x86_64.whl")
    else()
      message(FATAL_ERROR "Unsupported architecture for python output.")
    endif()
  else()
    message(FATAL_ERROR "Unsupported architecture for python output.")
  endif()

  if(DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
    set(ENV_COMMAND ${CMAKE_COMMAND} -E env
                    CMAKE_BUILD_PARALLEL_LEVEL=$ENV{CMAKE_BUILD_PARALLEL_LEVEL})
  else()
    set(ENV_COMMAND "")
  endif()

  set(PIP_WHEEL_OPTIONS
      -m
      pip
      wheel
      .
      --config-settings="--global-option=DISABLE_VCPKG=ON"
      --config-settings="--global-option=cmake_build_type=$<CONFIG>"
      --config-settings="--global-option=build_shared_libs=${BUILD_SHARED_LIBS}"
      --config-settings="--global-option=hdf5=${WITH_HDF5}"
      --config-settings="--global-option=-DCatch2_DIR=${Catch2_DIR}"
      --config-settings="--global-option=-DHDF5_DIR=${HDF5_DIR}"
      --config-settings="--global-option=-Dpybind11_DIR=${pybind11_DIR}"
      --config-settings="--global-option=-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
      --config-settings="--global-option=-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
      --wheel-dir
      ${CMAKE_CURRENT_BINARY_DIR}
      --no-clean
      -v)
  string(REPLACE ";" " " PIP_WHEEL_OPTIONS_ECHO "${PIP_WHEEL_OPTIONS}")

  if(Python3_EXECUTABLE_IS_DEBUG)
    set(Python3_EXECUTABLE_EXEC ${CMAKE_COMMAND} -E true)
  else()
    set(Python3_EXECUTABLE_EXEC ${ENV_COMMAND} ${Python3_EXECUTABLE})
  endif()
  if(NOT Python3_EXECUTABLE_DEBUG_IS_DEBUG)
    set(Python3_EXECUTABLE_DEBUG_EXEC ${CMAKE_COMMAND} -E true)
  else()
    set(Python3_EXECUTABLE_DEBUG_EXEC ${ENV_COMMAND}
                                      ${Python3_EXECUTABLE_DEBUG})
  endif()

  string(REPLACE ";" " " Python3_EXECUTABLE_EXEC_ECHO
                 "${Python3_EXECUTABLE_EXEC}")
  string(REPLACE ";" " " Python3_EXECUTABLE_DEBUG_EXEC_ECHO
                 "${Python3_EXECUTABLE_DEBUG_EXEC}")

  add_custom_target(
    UrxPythonWheel ALL
    COMMAND
      ${CMAKE_COMMAND} -E echo
      "$<IF:$<CONFIG:Debug>,${Python3_EXECUTABLE_DEBUG_EXEC_ECHO},${Python3_EXECUTABLE_EXEC_ECHO}> ${PIP_WHEEL_OPTIONS_ECHO}"
    COMMAND
      "$<IF:$<CONFIG:Debug>,${Python3_EXECUTABLE_DEBUG_EXEC},${Python3_EXECUTABLE_EXEC}>"
      ${PIP_WHEEL_OPTIONS}
    COMMAND_EXPAND_LISTS
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

  if(NOT Python3_EXECUTABLE_IS_DEBUG)
    install(
      FILES
        "$<$<NOT:$<CONFIG:Debug>>:${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_PYURX_OUTPUT}>"
      DESTINATION "share/${INSTALL_NAME}")
  endif()
  if(Python3_EXECUTABLE_DEBUG_IS_DEBUG)
    install(
      FILES
        "$<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_PYURX_OUTPUT}>"
      DESTINATION "share/${INSTALL_NAME}")
  endif()
endif()

install(
  TARGETS ${URX_PYTHON_LIBRARY}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

pybind11_add_module(
  UrxBindingsPython
  THIN_LTO
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings_module.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings_module2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/bindings_module.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
target_link_libraries(
  UrxBindingsPython PUBLIC ${PROJECT_NAME} Urx::UrxUtils
                           Urx::${URX_PYTHON_LIBRARY} pybind11::pybind11)

set_target_properties(UrxBindingsPython PROPERTIES OUTPUT_NAME "bindings")

if(ENABLE_PCH)
  target_precompile_headers(UrxBindingsPython PRIVATE ${URX_PCH}
                            ${URX_PCH_PYTHON})
endif()

install(
  TARGETS UrxBindingsPython
  EXPORT ${PROJECT_NAME}Targets
  COMPONENT bindings
  LIBRARY DESTINATION ${PYTHON_INSTALL_LIBDIR})
