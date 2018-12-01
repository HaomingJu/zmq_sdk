cmake_minimum_required(VERSION 2.8)

include(cmake/hobot_util.cmake)

project(zmq_sdk)

set(CMAKE_INSTALL_PREFIX ${_INSTALL_DIR})

add_subdirectory(test)
