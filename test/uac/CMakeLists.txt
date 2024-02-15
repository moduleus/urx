set(UAC_TEST_FILES test_build.cpp)

add_executable(UacClasses ${UAC_TEST_FILES})

target_link_libraries(UacClasses PRIVATE Uac::Uac Catch2::Catch2WithMain)

catch_discover_tests(UacClasses)
