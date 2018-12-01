set(SOURCE_FILES
    client_receive_ui.cc)

add_executable(client_receive_ui ${SOURCE_FILES})

target_link_libraries(client_receive_ui
    pthread
    Qt5Core
    Qt5Gui
    Qt5Widgets)

install(TARGETS client_receive_ui
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/test/client_receive_ui/data/test.jpeg
        DESTINATION data)
