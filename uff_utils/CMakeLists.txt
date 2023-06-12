# Set UFF sources
set(UFF_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)

# Target library ###
# Create target
set(UFF_UTILS_LIBRARY Uff_Utils)
add_library(${UFF_UTILS_LIBRARY} ${UFF_UTILS_SRCS})
add_library(${UFF_UTILS_LIBRARY}::${UFF_UTILS_LIBRARY} ALIAS
            ${UFF_UTILS_LIBRARY})

# Define include directories
target_include_directories(
  ${UFF_UTILS_LIBRARY}
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch}>
)
set_target_properties(${UFF_UTILS_LIBRARY}
                      PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${git_branch}")

# if(TARGET hdf5-${SEARCH_TYPE})
#   target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_HDF5)
#   target_link_libraries(${PROJECT_NAME} PUBLIC hdf5-${SEARCH_TYPE}
#                                                hdf5_cpp-${SEARCH_TYPE})
#   target_include_directories(
#     ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HDF5_CPP_SRC_SOURCE_DIR}>
#                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#   target_include_directories(${PROJECT_NAME}
#                              PUBLIC $<BUILD_INTERFACE:${HDF5_SRC_SOURCE_DIR}>)
#   target_include_directories(${PROJECT_NAME}
#                              PUBLIC $<BUILD_INTERFACE:${HDF5_BINARY_DIR}>)
# else()
find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})

target_link_libraries(
  ${UFF_UTILS_LIBRARY} PUBLIC Uff::Uff compiler_flags hdf5::hdf5-${SEARCH_TYPE}
                              hdf5::hdf5_cpp-${SEARCH_TYPE})
# endif()

# Install library ###

install(
  TARGETS ${UFF_UTILS_LIBRARY}
  EXPORT ${UFF_UTILS_LIBRARY}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch})

install(
  EXPORT ${UFF_UTILS_LIBRARY}Targets
  FILE ${UFF_UTILS_LIBRARY}Targets.cmake
  NAMESPACE ${UFF_UTILS_LIBRARY}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_UTILS_LIBRARY}-${git_branch})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${UFF_UTILS_LIBRARY}-${git_branch}
  FILES_MATCHING
  PATTERN "uff/*.hpp"
  PATTERN "uff/*.h"
  PATTERN ".git" EXCLUDE
  PATTERN "CI" EXCLUDE
  PATTERN "install" EXCLUDE
  PATTERN "build" EXCLUDE
  PATTERN "log.h" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${UFF_UTILS_LIBRARY}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${UFF_UTILS_LIBRARY}-${git_branch})
