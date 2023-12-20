set(URX_V0_2_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/aperture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aperture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/channel_data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/channel_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/linear_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/linear_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/perimeter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/perimeter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/position.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/position.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rca_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rca_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rotation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rotation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timed_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timed_event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/translation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/translation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_wave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_wave.h
    ${CMAKE_CURRENT_SOURCE_DIR}/types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/urx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.h
    ${CMAKE_CURRENT_SOURCE_DIR}/writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/writer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

add_library(Urxv0_2)
add_library(Urx::Urxv0_2 ALIAS Urxv0_2)
target_precompile_headers(Urxv0_2 PRIVATE ${URX_PCH} ${URX_PCH_IO_V2_0})

target_sources(Urxv0_2 PRIVATE ${URX_V0_2_SRCS})

target_include_directories(
  Urxv0_2
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)

if(WITH_HDF5)
  if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
  else()
    set(LIB_TYPE STATIC)
  endif()
  string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

  find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})

  target_compile_definitions(Urxv0_2 PUBLIC WITH_HDF5)
  target_link_libraries(Urxv0_2 PRIVATE hdf5::hdf5-${SEARCH_TYPE}
                                        hdf5::hdf5_cpp-${SEARCH_TYPE})
endif()

set_target_properties(Urxv0_2 PROPERTIES OUTPUT_NAME "Urxv0_2-${git_branch}")

install(
  TARGETS Urxv0_2
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})
