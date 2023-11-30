set(URX_TEST_FILES reader.cpp time_helper.cpp group_data_reader.cpp)

add_executable(UrxUtilsTest ${URX_TEST_FILES})

target_include_directories(UrxUtilsTest PUBLIC ${PROJECT_SOURCE_DIR}/urx
                                               ${PROJECT_SOURCE_DIR}/urx_utils)

target_link_libraries(
  UrxUtilsTest PRIVATE Urx::Urx Urx::UrxUtils ${HDF5_CXX_LIBRARIES}
                       Catch2::Catch2WithMain)

catch_discover_tests(UrxUtilsTest)
