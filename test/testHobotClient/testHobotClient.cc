/*
 * testHobotServer.cc
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <string>
#include "HobotNetwork/HobotZmqClient.h"
#include "base/base.h"

using namespace Modo;

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
  int bufflen = 2 * 1024 * 1024;

  for (int i = 0; i < 100000; ++i) {
    char buff[128];
    sprintf(buff, "world_%d", i);
    int ret = client.SendData(buff, strlen(buff));
    printf("SendData datasize = %d,buf=%s \n", ret, buff);
    //    size_t buflen = 10;
    //    // void *buf = malloc(buflen);
    //    char buff[128];
    //    int sz = client.RecvData(buf, 128);
    //    printf("RecvData datasize = %d,buf=%s \n", sz, buf);
    SLEEP(1);
  }
  client.Finish();
  return 0;
}
