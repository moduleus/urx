set(UFF_TEST_FILES reader.cpp time_helper.cpp group_data_reader.cpp)

add_executable(UffUtilsTest ${UFF_TEST_FILES})

target_include_directories(UffUtilsTest PUBLIC ${PROJECT_SOURCE_DIR}/uff
                                               ${PROJECT_SOURCE_DIR}/uff_utils)

target_link_libraries(
  UffUtilsTest PRIVATE Uff::Uff Uff::UffUtils ${HDF5_CXX_LIBRARIES}
                       Catch2::Catch2WithMain)

catch_discover_tests(UffUtilsTest)
