set(SOURCE_FILES
    client_receive.cc)

add_executable(client_receive ${SOURCE_FILES})

target_link_libraries(client_receive zmq)

install (TARGETS client_receive
        RUNTIME DESTINATION bin)

