set(UFF_TEST_FILES test_build.cpp test_vector.cpp test_transform.cpp test_version.cpp)

add_executable(UffClasses ${MAIN_CPP} ${UFF_TEST_FILES})
add_executable(UffTest::UffClasses ALIAS UffClasses)

target_include_directories(UffClasses PUBLIC ${PROJECT_SOURCE_DIR}/uff)

target_link_libraries(UffClasses PRIVATE Uff::Uff Catch2::Catch2)

catch_discover_tests(UffClasses)
