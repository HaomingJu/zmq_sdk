project(testTimeSync)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})


add_executable(${PROJECT_NAME}  ${CPP_FILES})

target_link_libraries(${PROJECT_NAME}
    HobotDataTransfer
    )
#set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)



