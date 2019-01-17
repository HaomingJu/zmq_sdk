include_directories(
    ${zmq_sdk_INCLUDE_DIRS})
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/src/symbols.lst")
add_subdirectory(HobotDataTransfer)
add_subdirectory(HobotNetwork)
add_subdirectory(utils)
add_subdirectory(HobotProtocol)
