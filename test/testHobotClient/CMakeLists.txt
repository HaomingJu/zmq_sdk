project(testHobotClient)

file(GLOB CPP_FILES *.cc)

add_executable(${PROJECT_NAME} ${CPP_FILES})



target_link_libraries(${PROJECT_NAME} HobotNetwork
    )


install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)



