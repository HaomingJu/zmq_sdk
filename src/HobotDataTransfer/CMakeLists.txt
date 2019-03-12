
message(">>>>>>>>>>>>>>.TRANSFER_PROJECT_DIRS: ${TRANSFER_PROJECT_DIRS}")
include_directories(${TRANSFER_PROJECT_DIRS})
add_subdirectory(main)
add_subdirectory(SendModule)
add_subdirectory(ReceiveModule)
add_subdirectory(DispatchModule)
