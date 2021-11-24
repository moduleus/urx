# Copyright Moduleus 2019

# Acquisition
set(uff_testing uffTestChannelData)
add_executable(${uff_testing} ${uff_testing}.cpp)
target_link_libraries(${uff_testing} PUBLIC ${UFF_LIBRARY})
add_test(${uff_testing} ${uff_testing})

