find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(URX_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS_FILES writer_test.cpp)

add_executable(UrxTestIo ${SRCS_FILES})
if(ENABLE_PCH)
  target_precompile_headers(UrxTestIo PRIVATE ${URX_PCH} ${URX_PCH_TEST})
endif()

target_link_libraries(
  UrxTestIo PRIVATE Urx::Urx Urx::UrxUtils ${HDF5_CXX_LIBRARIES}
                    Catch2::Catch2WithMain compiler_flags)

if(MSVC)
  target_compile_options(UrxTestIo PRIVATE /utf-8)
endif()

foreach(TESTI UrxTestIo)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()

catch_discover_tests(UrxTestIo)
