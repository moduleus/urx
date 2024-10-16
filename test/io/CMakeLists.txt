find_package(Catch2 REQUIRED)

set(SRCS_FILES writer_test.cpp)

add_executable(UrxTestIo ${SRCS_FILES})
if(ENABLE_PCH)
  target_precompile_headers(UrxTestIo PRIVATE ${URX_PCH} ${URX_PCH_TEST})
endif()

target_link_libraries(
  UrxTestIo PRIVATE Urx::Urx Urx::UrxUtils ${HDF5_CXX_LIBRARIES}
                    Catch2::Catch2WithMain Urx::UrxCompilerFlags)

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
