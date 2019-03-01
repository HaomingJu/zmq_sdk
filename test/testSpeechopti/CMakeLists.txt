project(testSpeech)

file(GLOB CPP_FILES *.cc)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})



if(${_PLATFORM} STREQUAL "linux" AND ${_ARCHITECTURE} STREQUAL "x86")
	add_executable(testPcClientOpti testPcClientOpti.cc )

	target_link_libraries(testPcClientOpti
	    HobotDataTransfer
	    hrscd)
	#set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "../lib")
	set_target_properties(testPcClientOpti PROPERTIES INSTALL_RPATH "../lib")

	install (TARGETS testPcClientOpti
	    RUNTIME DESTINATION bin)
endif()


