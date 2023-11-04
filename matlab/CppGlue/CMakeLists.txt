
set(MATLAB_CPP_GLUE_SRCS
  acquisition.cpp)

add_library(MatlabCppGlueAcquisition SHARED ${MATLAB_CPP_GLUE_SRCS})

target_include_directories(MatlabCppGlueAcquisition PUBLIC ${PROJECT_SOURCE_DIR}/uff)

target_link_libraries(MatlabCppGlueAcquisition PRIVATE Uff::Uff)
