project(utils)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

install(FILES "${zmq_sdk_INCLUDE_DIRS}/utils/CommunicationProtocol.h" DESTINATION include)
