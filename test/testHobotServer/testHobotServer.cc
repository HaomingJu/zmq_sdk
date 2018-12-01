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
#include "HobotNetwork/HobotServerBase.h"
#include "HobotNetwork/HobotZmqServer.h"
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
  HobotZmqServer servrer;
  servrer.Init(ipstr.c_str());
  char str[] = "hello world";
  printf("begin Send \n");
  while (true) {
    printf("begin Send=%s \n", str);
    servrer.SendData(str, strlen(str), nullptr);
    printf("end Send=%s \n", str);
    sleep(1);
  }

  return 0;
}
