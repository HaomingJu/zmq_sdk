project(TimeSync)

file(GLOB CPP_FILES *.cc)
message(">>>>>>>>>>>>>>zmq_sdk_INCLUDE_DIRS:${zmq_sdk_INCLUDE_DIRS}")
include_directories(
    ${zmq_sdk_INCLUDE_DIRS})
set(library_list
    zmq
    )

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})
