/*
 * testTranClient.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "base/base.h"
//#include <zmq.h>
//#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "TimeSync/TimeSync.h"

#define TIME_LONG 8
#define AUDIO_LEN 4

int sync_time() {
  std::cout << "begin sync_time" << std::endl;
  Modo::TimeSync::ClientSyncTime("192.168.1.10", 5);
  int64_t stamp = Modo::TimeSync::GetEpochTimeMs();
  int64_t offset = Modo::TimeSync::GetDeviceTimeOffset();
  std::cout << "GetEpochTimeMs, stamp [" << stamp << "," << offset << "]"
            << std::endl;
  // std::cout << "GetEpochTimeMs, stamp = " << stamp << std::endl;
  return 0;
}

int main(int argc, char **argv) {
  std::string ip;
  std::string port;
  std::string ipstr;
  if (argc < 3) {
    ipstr = "tcp://localhost:5555";
  } else {
    ip = argv[1];
    port = argv[2];
    ipstr = "tcp://" + ip + ":" + port;
  }

  int i = 1;
  while (true) {
    // int64_t stamp = Modo::TimeSync::GetEpochTimeMs();
    // int64_t stamp1 = Modo::TimeSync::GetEpochTimeMs(Modo::GetSysStamp());
    // int64_t offset = Modo::TimeSync::GetDeviceTimeOffset();
    // std::cout << "GetEpochTimeMs, stamp [" << stamp << "," << offset << "]"
    //           << std::endl;
    // if (i % 20 == 0) {
    sync_time();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    // }
    i++;
  }

  return 0;
}
