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
#include "HobotNetwork/HobotClientBase.h"
#include "HobotNetwork/HobotZmqClient.h"
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
  HobotZmqClient client;
  client.Init(ipstr.c_str());
  char buf[100];
  while (true) {
    client.RecvData(nullptr, 0);
    client.RecvData(buf, 90);
    printf("RecvData=%s \n", buf);
    buf[0] = '\0';
    // sleep(1);
  }

  return 0;
}
