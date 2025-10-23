set(URX_TEST_FILES
    clone_generic_test.h
    clone_generic_test.cpp
    compare.cpp
    cpp.cpp
    validator.cpp
    time_helper.cpp
    group_data_reader.cpp
    probe_helper.cpp)

add_executable(UrxUtilsTest ${URX_TEST_FILES})

target_link_libraries(UrxUtilsTest PRIVATE Urx::Urx Urx::UrxUtils
                                           Catch2::Catch2WithMain)

foreach(TESTI UrxUtilsTest)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()

catch_discover_tests(UrxUtilsTest)
