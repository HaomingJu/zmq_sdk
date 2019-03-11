/*
 * testMonitor.cc
 *
 *  Created on: Mar 8, 2019
 *      Author: user
 */



#include <stdio.h>
#include <unistd.h>
#include <zmq.h>
#include <string>
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

  printf("Connecting to hello world serve:%s\n", ipstr.c_str());

  /*创建一个新的上下文*/
  void *context = zmq_ctx_new();
  void *requester = zmq_socket(context, ZMQ_REQ);
  int rc = zmq_socket_monitor (client, "inproc://monitor-client", ZMQ_EVENT_ALL);
  zmq_close(requester);
  zmq_ctx_destroy(context);

  return 0;
}
