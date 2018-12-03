add_subdirectory(testClient)
add_subdirectory(testServer)

if(${platform} STREQUAL "linux" AND ${architecture} STREQUAL "x86" AND ${vendor} STREQUAL "default")
    #add_subdirectory(client_receive_ui)
endif()

add_subdirectory(testHobotServer)
add_subdirectory(testHobotClient)
