set(URX_TEST_FILES
    test_build.cpp
    test_DoubleNan.cpp
    test_ElementGeometry.cpp
    test_RawData.cpp
    test_Vector.cpp
    test_Transform.cpp
    test_Version.cpp)

add_executable(UrxClasses ${URX_TEST_FILES})
if(ENABLE_PCH)
  target_precompile_headers(UrxClasses PRIVATE ${URX_PCH} ${URX_PCH_TEST})
endif()

target_link_libraries(UrxClasses PRIVATE Urx::Urx Catch2::Catch2WithMain)

catch_discover_tests(UrxClasses)
