if(WITH_HDF5)
  set(URX_WITH_HDF5 1)
endif()
if(WITH_PYTHON)
  set(URX_WITH_PYTHON 1)
endif()

configure_file(config.h.in config.h @ONLY)
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/config.h")

set(URX_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/compare.h
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/double_nan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/raw_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.h)

add_library(${PROJECT_NAME} INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME} INTERFACE FILE_SET HEADERS BASE_DIRS
                            ${CMAKE_CURRENT_SOURCE_DIR}/.. FILES ${URX_SRCS})
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config.h")

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${INSTALL_NAME}")

add_subdirectory("utils")

# Install library ###

# Create cmake config files ###
configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${INSTALL_NAME})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/config.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/urx")

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${INSTALL_NAME})
