cmake_minimum_required(VERSION 2.8)


include(cmake/hobot_util.cmake)
project(zmq_sdk)



message("config types: ${CMAKE_CONFIGURATION_TYPES}")

set(CMAKE_BUILD_TYPE Release)


include_directories(
        include
        src
)

set(SOURCE_FILES
        src/myapi.cpp
        include/zmq_sdk/myapi.h
        )

add_library(zmq_sdk SHARED ${SOURCE_FILES})

# add_subdirectory(test)

set(OUTPUT_ROOT ${PROJECT_SOURCE_DIR}/output)

set(MY_OUTPUT_ROOT ${OUTPUT_ROOT}/zmq_sdk)
install(TARGETS zmq_sdk
    DESTINATION ${MY_OUTPUT_ROOT}/lib)
install(FILES ${PROJECT_SOURCE_DIR}/include/zmq_sdk/myapi.h
    DESTINATION ${MY_OUTPUT_ROOT}/include/zmq_sdk/)
