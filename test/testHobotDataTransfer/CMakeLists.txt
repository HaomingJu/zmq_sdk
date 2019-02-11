project(testTransfer)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

add_executable(testTranServer testTranServer.cc)
add_executable(testTranClient testTranClient.cc)

target_link_libraries(testTranServer
    HobotDataTransfer)
target_link_libraries(testTranClient
    HobotDataTransfer)
#set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")
set_target_properties(testTranServer PROPERTIES INSTALL_RPATH "../lib")
set_target_properties(testTranClient PROPERTIES INSTALL_RPATH "../lib")
install (TARGETS testTranServer
    RUNTIME DESTINATION bin)

install (TARGETS testTranClient
    RUNTIME DESTINATION bin)
