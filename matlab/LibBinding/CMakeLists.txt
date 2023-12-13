
set(MATLAB_CPP_GLUE_SRCS
  UffLibBinding.cpp)

add_library(UffLibBinding SHARED ${MATLAB_CPP_GLUE_SRCS})

target_include_directories(UffLibBinding PUBLIC ${PROJECT_SOURCE_DIR}/uff)

target_link_libraries(UffLibBinding PRIVATE Uff::Uff)
