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
  int i = 0;
  while (true) {
    char buff[128];
    //  sprintf(buff, "world_%d", i);

    // printf("begin Send \n");
    // printf("begin Send=%s \n", str);
    // int ret = server.SendData(buff, strlen(buff));
    int sz = server.RecvData(buff, 128);
    printf("recv datasize = %d,buf = %s\n", sz, buff);
    // printf("Send=%s\n", buff);
    sleep(1);
    i++;
  }
  server.Finish();
  return 0;
}
