cmake_minimum_required(VERSION 2.8)

include(cmake/hobot_util.cmake)

project(zmq_sdk)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC -Wall -g")
include_directories(./src/)
set(CMAKE_INSTALL_PREFIX ${_INSTALL_DIR})
add_subdirectory(src)
add_subdirectory(test)
