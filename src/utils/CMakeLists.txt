project(utils)

file(GLOB CPP_FILES *.cc)


set(library_list
     zmq
     hobotlog
    )


add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})
