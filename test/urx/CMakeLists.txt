set(URX_TEST_FILES test_build.cpp test_double_nan.cpp test_vector.cpp
                   test_transform.cpp test_version.cpp)

add_executable(UrxClasses ${URX_TEST_FILES})

target_include_directories(UrxClasses PUBLIC ${PROJECT_SOURCE_DIR}/urx)

target_link_libraries(UrxClasses PRIVATE Urx::Urx Catch2::Catch2WithMain)

catch_discover_tests(UrxClasses)
