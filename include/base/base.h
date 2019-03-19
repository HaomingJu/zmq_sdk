/*
 * base.h
 *
 *  Created on: Mar 12, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_INCLUDE_BASE_BASE_H_
#define ZMQ_SDK_INCLUDE_BASE_BASE_H_
#include <iostream>
#include <thread>
#include "hobotlog/hobotlog.hpp"
#ifndef WIN32
#include <unistd.h>
#define SLEEP sleep
#else
#include <windows.h>
#include <winsock2.h>

#define SLEEP Sleep
#endif
#define SLEEPMS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define TRANSFER_TIMEOUT -1
#define TRANSFER_UNCONNECT_ERROR -2
#define TRANSFER_BUG_ERROR -3
#define TRANSFER_PTR_NULL -4
#endif /* ZMQ_SDK_INCLUDE_BASE_BASE_H_ */
