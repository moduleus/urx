
set(MATLAB_CPP_GLUE_SRCS
  UrxLibBinding.cpp)

add_library(UrxLibBinding SHARED ${MATLAB_CPP_GLUE_SRCS})

target_include_directories(UrxLibBinding PUBLIC ${PROJECT_SOURCE_DIR}/urx)

target_link_libraries(UrxLibBinding PRIVATE Urx::Urx)
