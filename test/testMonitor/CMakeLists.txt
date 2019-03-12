project(testMonitor)

file(GLOB CPP_FILES *.cc)

add_executable(${PROJECT_NAME} ${CPP_FILES})


target_link_libraries(${PROJECT_NAME} ${ZMQ_LIB})

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)



