project(HobotNetwork)

file(GLOB CPP_FILES *.cc)


set(library_list
    utils
    zmq
    hobotlog)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${PROJECT_SOURCE_DIR}/symbols.lst")

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})
#add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(FILES "${zmq_sdk_INCLUDE_DIRS}/HobotNetwork/HobotZmqClient.h" DESTINATION include)
install(FILES "${zmq_sdk_INCLUDE_DIRS}/HobotNetwork/HobotZmqServer.h" DESTINATION include)


target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})
