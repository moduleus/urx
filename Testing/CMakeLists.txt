if(TARGET Uff::Uff_Utils)
  foreach(TESTI uff_test_time_helper)
    add_executable(${TESTI} ${TESTI}.cpp)
    target_link_libraries(${TESTI} PRIVATE Uff::Uff Uff::Uff_Utils)
    add_test(NAME ${TESTI} COMMAND $<TARGET_FILE:${TESTI}>)
    if(WIN32 AND BUILD_SHARED_LIBS)
      add_custom_command(
        TARGET ${TESTI}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
        COMMAND_EXPAND_LISTS)
    endif()
  endforeach()
endif()

if(TARGET Uff::Uff_Utils)
  add_subdirectory(utils)
endif()
