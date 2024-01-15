cmake_policy(SET CMP0079 NEW)
target_compile_definitions(UrxUtils PRIVATE WITH_PYTHON)
target_link_libraries(UrxUtils PRIVATE Python3::Python pybind11::headers)

pybind11_add_module(BindingsPython bindings.cpp bindings_slow.cpp bindings.h
                    CMakeLists.txt)
target_link_libraries(BindingsPython PUBLIC ${PROJECT_NAME} Urx::UrxUtils)

set_target_properties(BindingsPython PROPERTIES OUTPUT_NAME "bindings")

target_precompile_headers(BindingsPython PRIVATE ${URX_PCH} ${URX_PCH_PYTHON})

install(
  TARGETS BindingsPython
  COMPONENT bindings
  LIBRARY DESTINATION ${PYTHON_INSTALL_LIBDIR})
