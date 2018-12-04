cmake_minimum_required(VERSION 2.8)

include(cmake/hobot_util.cmake)

project(zmq_sdk)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC -Wall -g")

include_directories(./src/)

set(CMAKE_INSTALL_PREFIX ${_INSTALL_DIR})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

add_subdirectory(src)
add_subdirectory(test)

if(${platform} STREQUAL "linux" AND ${architecture} STREQUAL "x86" AND ${vendor} STREQUAL "default")
    add_subdirectory(example)
endif()
