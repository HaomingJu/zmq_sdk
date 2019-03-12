/*
 * base.h
 *
 *  Created on: Mar 12, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_INCLUDE_BASE_BASE_H_
#define ZMQ_SDK_INCLUDE_BASE_BASE_H_
#include <iostream>
#include "hobotlog/hobotlog.hpp"
#ifndef WIN32
#include <unistd.h>
#define SLEEP sleep
#else
#include <windows.h>
#include <winsock2.h>

#define SLEEP Sleep
#endif


#endif /* ZMQ_SDK_INCLUDE_BASE_BASE_H_ */
