set(UFF_TEST_FILES reader.cpp time_helper.cpp group_data_reader.cpp)

add_executable(UffUtils ${MAIN_CPP} ${UFF_TEST_FILES})
add_executable(UffTest::UffUtils ALIAS UffUtils)

target_include_directories(UffUtils PUBLIC ${PROJECT_SOURCE_DIR}/uff
                                           ${PROJECT_SOURCE_DIR}/uff_utils)

target_link_libraries(UffUtils PRIVATE Uff::Uff Uff::Uff_Utils
                                       ${HDF5_CXX_LIBRARIES} Catch2::Catch2)

catch_discover_tests(UffUtils)
