find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(URX_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# set(SRCS_FILES reader.cpp operator.cpp)

# if(TARGET Urx::UrxUtils)
#   foreach(TESTI urx_test_time_helper)
#     add_executable(${TESTI} ${TESTI}.cpp)
#     target_link_libraries(${TESTI} PRIVATE Urx::Urx Urx::UrxUtils)
#     add_test(NAME ${TESTI} COMMAND $<TARGET_FILE:${TESTI}>)
#     if(WIN32 AND BUILD_SHARED_LIBS)
#       add_custom_command(
#         TARGET ${TESTI}
#         POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                 $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
#         COMMAND_EXPAND_LISTS)
#     endif()
#   endforeach()
# endif()

add_subdirectory(utils)

add_subdirectory(urx)

add_subdirectory(uac)

if(WITH_HDF5)
  add_subdirectory(io)
endif()
