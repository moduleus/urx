set(UFF_TEST_FILES test_build.cpp test_double_nan.cpp test_vector.cpp
                   test_transform.cpp test_version.cpp)

add_executable(UffClasses ${UFF_TEST_FILES})

target_include_directories(UffClasses PUBLIC ${PROJECT_SOURCE_DIR}/uff)

target_link_libraries(UffClasses PRIVATE Uff::Uff Catch2::Catch2WithMain)

catch_discover_tests(UffClasses)
