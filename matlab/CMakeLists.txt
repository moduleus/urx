install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/+urx"
        DESTINATION ${MATLAB_INSTALL_LIBDIR})

add_subdirectory("LibBinding")
