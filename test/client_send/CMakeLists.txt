set(SOURCE_FILES
    client_send.cc)

add_executable(client_send ${SOURCE_FILES})

target_link_libraries(client_send pthread zmq)

install (TARGETS client_send
        RUNTIME DESTINATION bin)
