project(testHobotClient)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${CPP_FILES})


target_link_libraries(${PROJECT_NAME}
    HobotDataTransfer
	)


install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)
