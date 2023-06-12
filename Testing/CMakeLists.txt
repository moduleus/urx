foreach(TESTI uffTestChannelData)
  add_executable(${TESTI} ${TESTI}.cpp)
  target_link_libraries(${TESTI} PRIVATE ${UFF_LIBRARY})
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

add_subdirectory(utils)
