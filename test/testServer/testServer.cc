/*
 * testServer.cc
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */

#include <assert.h>
#include <stdio.h>
#include <zmq.h>
#include <string>
#include "base/base.h"

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
  printf("Listening:%s\n", ipstr.c_str());
  //  Socket to talk to clients
  void *context = zmq_ctx_new();
  void *responder = zmq_socket(context, ZMQ_DEALER);
  int rc = zmq_bind(responder, "tcp://*:5555");
  assert(rc == 0);

  while (1) {
    char buffer[128] = {0};
    int ret = zmq_recv(responder, buffer, 10, ZMQ_DONTWAIT);
    printf("Received %s,%d\n", buffer, ret);
    // sleep(1);  //  Do some 'work'
    // zmq_send(responder, "World", 5, 0);
  }
  return 0;
}
