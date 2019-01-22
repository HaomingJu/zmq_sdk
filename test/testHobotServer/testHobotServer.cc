/*
 * testHobotServer.cc
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include <string>
#include "HobotNetwork/HobotNetworkInstance.h"
#include "HobotNetwork/HobotZmqServer.h"

using namespace Modo;
int main(int argc, char **argv) {
  std::string ip;
  std::string port;
  std::string ipstr;
  if (argc < 3) {
    ipstr = "tcp://*:5555";
  } else {
    ip = argv[1];
    port = argv[2];
    ipstr = "tcp://" + ip + ":" + port;
  }
  HobotZmqServer server;
  server.Init(ipstr.c_str());
  char str[] = "World";
  while (true) {
    size_t buflen = 1024 * 1024;
    void *buf = malloc(buflen);
    memset(buf, 0, buflen);
    int sz = server.RecvData(buf, buflen);
    printf("recv datasize = %d", sz);
    // printf("begin Send \n");
    // printf("begin Send=%s \n", str);
    server.SendData("World", 6);
    printf("Send=%s \n", str);
    sleep(1);
  }
  server.Finish();
  return 0;
}
