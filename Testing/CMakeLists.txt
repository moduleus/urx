# Copyright Moduleus 2019

# ChannelData
add_executable(uffTestChannelData uffTestChannelData.cpp)
target_link_libraries(uffTestChannelData PRIVATE ${PROJECT_NAME})
add_test(uffTestChannelData uffTestChannelData)
