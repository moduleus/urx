set(MATLAB_CPP_GLUE_SRCS UrxLibBinding.cpp UrxLibBinding.h CMakeLists.txt)

add_library(UrxLibBinding SHARED ${MATLAB_CPP_GLUE_SRCS})
add_library(urx::UrxLibBinding ALIAS UrxLibBinding)

target_include_directories(UrxLibBinding PUBLIC ${PROJECT_SOURCE_DIR}/urx)

target_link_libraries(UrxLibBinding PRIVATE Urx::Urx)

if(WITH_HDF5)
  target_link_libraries(UrxLibBinding PRIVATE Urx::UrxUtils)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET UrxLibBinding
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:UrxLibBinding>
      $<TARGET_FILE_DIR:UrxLibBinding>
    COMMAND_EXPAND_LISTS)
endif()
