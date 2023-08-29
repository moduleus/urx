# Set UFF sources
set(UFF_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/aperture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/destination_link.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/igroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/linear_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rca_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/super_group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timed_event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_in.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_location.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_out.h
    ${CMAKE_CURRENT_SOURCE_DIR}/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uff.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

# Configure a header file to pass the version number only
configure_file(uff.h.in uff.h)

# Target library ### Create target
set(UFF_LIBRARY ${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Define include directories
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}>
)
set_target_properties(${PROJECT_NAME}
                      PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${git_branch}")

# Install library ###

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch})

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${git_branch}
  FILES_MATCHING
  PATTERN "uff/*.hpp"
  PATTERN "uff/*.h"
  PATTERN ".git" EXCLUDE
  PATTERN "CI" EXCLUDE
  PATTERN "install" EXCLUDE
  PATTERN "build" EXCLUDE
  PATTERN "log.h" EXCLUDE)
