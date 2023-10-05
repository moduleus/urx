pybind11_add_module(BindingsPython bindings.cpp)
target_link_libraries(BindingsPython PUBLIC ${PROJECT_NAME})

set_target_properties(BindingsPython PROPERTIES OUTPUT_NAME "bindings")

install(
  TARGETS BindingsPython
  COMPONENT bindings
  LIBRARY DESTINATION ${PYTHON_INSTALL_LIBDIR})
