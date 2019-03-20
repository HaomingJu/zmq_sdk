project(testReceivMatrix)

file(GLOB CPP_FILES *.cc)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-R./dms_rt/lib/,-R./dms_rt/,--unresolved-symbols=ignore-in-shared-libs ")

add_executable(${PROJECT_NAME} ${CPP_FILES})



target_link_libraries(${PROJECT_NAME} HobotDataTransfer protobuf opencv_world)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin)
