# Set UAC sources
set(UAC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/compare.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/destination_link.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hw_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/igroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/super_group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uac.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

# Target library ### Create target
set(UAC_LIBRARY "Uac")
add_library(${UAC_LIBRARY} INTERFACE ${UAC_SRCS})
add_library(${UAC_LIBRARY}::${UAC_LIBRARY} ALIAS ${UAC_LIBRARY})

target_link_libraries(${UAC_LIBRARY} INTERFACE Urx::Urx)

# Define include directories
target_include_directories(
  ${UAC_LIBRARY}
  INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(${UAC_LIBRARY} PROPERTIES OUTPUT_NAME
                                                "${UAC_LIBRARY}-${git_branch}")

# add_subdirectory("utils")

# Install library ###

# Create cmake config files ###
configure_package_config_file(
  ${UAC_LIBRARY}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${UAC_LIBRARY}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${UAC_LIBRARY}-${git_branch})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${UAC_LIBRARY}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(
  TARGETS ${UAC_LIBRARY}
  EXPORT ${UAC_LIBRARY}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(
  EXPORT ${UAC_LIBRARY}Targets
  FILE ${UAC_LIBRARY}Targets.cmake
  NAMESPACE ${UAC_LIBRARY}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${INSTALL_NAME})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}/urx
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h"
  PATTERN ".git" EXCLUDE
  PATTERN "CI" EXCLUDE
  PATTERN "install" EXCLUDE
  PATTERN "build" EXCLUDE
  PATTERN "log.h" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${UAC_LIBRARY}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${UAC_LIBRARY}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UAC_LIBRARY}-${git_branch})
