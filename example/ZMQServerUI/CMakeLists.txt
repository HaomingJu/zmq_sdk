project(ZMQServerUI)

include_directories(
    ${zmq_sdk_INCLUDE_DIRS})

#file(GLOB CPP_FILES *.cc)

set(CPP_FILES
    mainwindow.cc
    moc_mainwindow.cc
    ZMQServerUi.cc)


set(library_list
    HobotNetwork
    Qt5Widgets
    pthread
    Qt5Core
    Qt5Gui)

add_executable(${PROJECT_NAME} ${CPP_FILES})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)

target_link_libraries(${PROJECT_NAME}
    ${library_list})
