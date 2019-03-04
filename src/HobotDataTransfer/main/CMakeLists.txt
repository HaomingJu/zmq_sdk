project(HobotDataTransfer)

set(CPP_FILES HobotDataTransfer.cc)
#message(">>>>>>>>>>>>TRANSFER_PROJECT_DIRS : ${TRANSFER_PROJECT_DIRS}")
include_directories(${TRANSFER_PROJECT_DIRS})
if (${_PLATFORM} STREQUAL "android")
    set(LOG_LIB log)
else ()
    set(LOG_LIB "")
endif()
set(library_list
    hobotlog
    hobotsdk
    hobotthread
    SendModule
    ReceiveModule
    DispatchModule
    utils
    HobotNetwork
    HobotProtocol
    TimeSync
    ${LOG_LIB}
    )


add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)