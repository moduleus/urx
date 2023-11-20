find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(UFF_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS_FILES reader_test.cpp writer_test.cpp)

add_executable(UffIo ${SRCS_FILES})

target_link_libraries(
  UffIo PRIVATE Uff::Uff Uff::Uff_Utils ${HDF5_CXX_LIBRARIES}
                Catch2::Catch2WithMain compiler_flags)

foreach(TESTI UffIo)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()

catch_discover_tests(UffIo)
