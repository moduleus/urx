find_package(Catch2 REQUIRED)

set(SRCS_FILES writer_test.cpp ../resources.cpp ../resources.h)

add_executable(UrxTestIo ${SRCS_FILES})
if(ENABLE_PCH)
  target_precompile_headers(UrxTestIo PRIVATE ${URX_PCH} ${URX_PCH_TEST})
endif()

if(URX_HDF5_LINK_SHARED)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
if(NOT TARGET hdf5::hdf5-${SEARCH_TYPE})
  find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
endif()

target_link_libraries(
  UrxTestIo
  PRIVATE Urx::Urx Urx::UrxUtils hdf5::hdf5-${SEARCH_TYPE}
          hdf5::hdf5_cpp-${SEARCH_TYPE} Catch2::Catch2 Urx::UrxCompilerFlags)
target_compile_definitions(UrxTestIo PRIVATE TEST_DATA_PATH="${TEST_DATA_PATH}")

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
