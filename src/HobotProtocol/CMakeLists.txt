project(HobotProtocol)

file(GLOB CPP_FILES *.cc)

set(library_list
     ${ZMQ_LIB}
     ${LOG_LIB}
    )

add_library(${PROJECT_NAME} STATIC ${CPP_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})