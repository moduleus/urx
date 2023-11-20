find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(UFF_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MAIN_CPP ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# set(SRCS_FILES reader.cpp operator.cpp)

# if(TARGET Uff::Uff_Utils)
#   foreach(TESTI uff_test_time_helper)
#     add_executable(${TESTI} ${TESTI}.cpp)
#     target_link_libraries(${TESTI} PRIVATE Uff::Uff Uff::Uff_Utils)
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

if(TARGET Uff::Uff_Utils)
  add_subdirectory(utils)
endif()

add_subdirectory(uff)

if(WITH_HDF5)
  add_subdirectory(io)
endif()
