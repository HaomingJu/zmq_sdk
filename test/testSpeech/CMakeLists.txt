project(testSpeech)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

add_executable(testMatrixServer testMatrixServer.cc )


target_link_libraries(testMatrixServer
    HobotDataTransfer)
set_target_properties(testMatrixServer PROPERTIES INSTALL_RPATH "../lib")
install (TARGETS testMatrixServer
    RUNTIME DESTINATION bin)


if(${_PLATFORM} STREQUAL "linux" AND ${_ARCHITECTURE} STREQUAL "x86")
	add_executable(testPcClient testPcClient.cc time_sync/time_sync.cpp)

	target_link_libraries(testPcClient
	    HobotDataTransfer
	    hrscd)
	#set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")
	set_target_properties(testPcClient PROPERTIES INSTALL_RPATH "../lib")

	install (TARGETS testPcClient
	    RUNTIME DESTINATION bin)
endif()


