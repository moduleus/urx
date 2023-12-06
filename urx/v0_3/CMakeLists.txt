set(URX_V0_3_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/aperture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group_link.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/igroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/polarity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/position.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rotation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sampling_type.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timed_event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/translation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_destination.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_in.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_out.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_source.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger_type.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave_type.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.h)

add_library(Urxv0_3 INTERFACE)
add_library(Urx::Urxv0_3 ALIAS Urxv0_3)

target_sources(Urxv0_3 PRIVATE ${URX_V0_3_SRCS})

target_include_directories(
  Urxv0_3
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)

set_target_properties(Urxv0_3 PROPERTIES OUTPUT_NAME "Urxv0_3-${git_branch}")

install(
  TARGETS Urxv0_3
  EXPORT Urxv0_3Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(
  EXPORT Urxv0_3Targets
  FILE Urxv0_3Targets.cmake
  NAMESPACE Urx::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${INSTALL_NAME})
