cmake_policy(SET CMP0079 NEW)
target_link_libraries(UrxUtils PUBLIC Python3::Python pybind11::headers)

set(URX_PYTHON_LIBRARY UrxPython)
set(URX_PYTHON_LIBRARY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bindings_public.cpp)
set(URX_PYTHON_LIBRARY_H ${CMAKE_CURRENT_SOURCE_DIR}/urx/python/bindings.h)
add_library(${URX_PYTHON_LIBRARY})
add_library(Urx::${URX_PYTHON_LIBRARY} ALIAS ${URX_PYTHON_LIBRARY})
target_link_libraries(${URX_PYTHON_LIBRARY}
                      PUBLIC Python3::Python pybind11::headers Urx::UrxUtils)
if(MSVC)
  target_compile_options(${URX_PYTHON_LIBRARY} PRIVATE /bigobj)
endif()
target_sources(
  ${URX_PYTHON_LIBRARY}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
         ${URX_PYTHON_LIBRARY_H}
  PRIVATE ${URX_PYTHON_LIBRARY_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
if(ENABLE_PCH)
  target_precompile_headers(${URX_PYTHON_LIBRARY} PRIVATE ${URX_PCH}
                            ${URX_PCH_PYTHON})
endif()
target_include_directories(
  ${URX_PYTHON_LIBRARY}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(
  ${URX_PYTHON_LIBRARY} PROPERTIES OUTPUT_NAME
                                   "${URX_PYTHON_LIBRARY}-${git_branch}")

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(
  TARGETS ${URX_PYTHON_LIBRARY}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

pybind11_add_module(UrxBindingsPython bindings.cpp CMakeLists.txt)
target_link_libraries(UrxBindingsPython PUBLIC ${PROJECT_NAME} Urx::UrxUtils
                                               Urx::${URX_PYTHON_LIBRARY})

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
