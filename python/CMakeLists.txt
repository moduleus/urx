pybind11_add_module(BindingsPython bindings.cpp)
target_link_libraries(BindingsPython PUBLIC ${PROJECT_NAME} Urx::UrxUtils)

set_target_properties(BindingsPython PROPERTIES OUTPUT_NAME "bindings")

target_precompile_headers(BindingsPython PRIVATE ${URX_PCH} ${URX_PCH_PYTHON})

install(
  TARGETS BindingsPython
  COMPONENT bindings
  LIBRARY DESTINATION ${PYTHON_INSTALL_LIBDIR})
