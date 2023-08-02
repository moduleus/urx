find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

set(UFF_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MAIN_CPP ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(SRCS_FILES reader.cpp operator.cpp)

add_executable(UffTest ${MAIN_CPP} ${SRCS_FILES})

target_include_directories(UffTest PUBLIC ${PROJECT_SOURCE_DIR}/uff)

target_link_libraries(UffTest PRIVATE Uff::Uff Uff::Uff_Utils
                                      ${HDF5_CXX_LIBRARIES} Catch2::Catch2)

catch_discover_tests(UffTest)
