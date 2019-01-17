project(HobotDataTransfer)

set(CPP_FILES HobotDataTransfer.cc)
#message(">>>>>>>>>>>>TRANSFER_PROJECT_DIRS : ${TRANSFER_PROJECT_DIRS}")
include_directories(${TRANSFER_PROJECT_DIRS})
set(library_list
    hobotlog
    hobotthread
    hobotsdk
    SendModule
    ReceiveModule
    DispatchModule
    utils
    HobotNetwork
    HobotProtocol
    )


add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)