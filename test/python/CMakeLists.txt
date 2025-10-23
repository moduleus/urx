install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/dataset_gen.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_acquisition.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_dataset.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_double_nan.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_element_geometry.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_element.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_event.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_excitation.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_group_data.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_group.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_impulse_response.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_probe.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_receive_setup.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_std_vector.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_transform.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_transmit_setup.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_utils.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_vector.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_version.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_wave.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bindings_clone.py
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${INSTALL_NAME}/python)
