find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(URX_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS_FILES writer_test.cpp upgrade_test.cpp)

add_executable(UrxIo ${SRCS_FILES})
target_precompile_headers(UrxIo PRIVATE ${URX_PCH} ${URX_PCH_TEST})

target_link_libraries(
  UrxIo PRIVATE Urx::Urx Urx::UrxUtils Urx::Urxv0_5 ${HDF5_CXX_LIBRARIES}
                Catch2::Catch2WithMain compiler_flags)

target_compile_definitions(
  UrxIo PRIVATE TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

if(MSVC)
  target_compile_options(UrxIo PRIVATE /utf-8)
endif()

foreach(TESTI UrxIo)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()

catch_discover_tests(UrxIo)
