project(testReceivMatrix)

file(GLOB CPP_FILES *.cc)

add_executable(${PROJECT_NAME} ${CPP_FILES})



target_link_libraries(${PROJECT_NAME} HobotDataTransfer protobuf)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)
