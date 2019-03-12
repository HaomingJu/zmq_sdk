project(HobotDataTransfer)

set(CPP_FILES HobotDataTransfer.cc)
#message(">>>>>>>>>>>>TRANSFER_PROJECT_DIRS : ${TRANSFER_PROJECT_DIRS}")
include_directories(${TRANSFER_PROJECT_DIRS})
message(">>>>>>>>>>>>>_PLATFORM:${_PLATFORM}")


message(">>>>>>>>>>>>>LOG_LIB:${LOG_LIB}")
set(library_list
	hobotsdk
    hobotthread
    SendModule
    ReceiveModule
    DispatchModule
   # utils
    HobotNetwork
    HobotProtocol
    TimeSync
    ${LOG_LIB}
    ${WIN_LIB}
    )

add_library(HobotDataTransferStatic STATIC ${CPP_FILES})
add_library(HobotDataTransfer SHARED ${CPP_FILES})
SET_TARGET_PROPERTIES (HobotDataTransferStatic PROPERTIES OUTPUT_NAME "HobotDataTransfer")
GET_TARGET_PROPERTY (OUTPUT_VALUE HobotDataTransferStatic OUTPUT_NAME)
SET_TARGET_PROPERTIES (HobotDataTransferStatic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (HobotDataTransfer PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(
    ${PROJECT_NAME}
    ${library_list})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)