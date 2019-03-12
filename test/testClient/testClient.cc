/*
 * testClient.cc
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */


#include <stdio.h>
#include <string.h>
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
  void *requester = zmq_socket(context, ZMQ_DEALER);
  int zmq_sndhwm = 1;
  zmq_setsockopt(requester, ZMQ_SNDHWM, &zmq_sndhwm, sizeof(int));
  /*通过tcp协议，5555端口，连接本机服务端*/
  zmq_connect(requester, ipstr.c_str());

  int i = 0;
  while (true) {
    char buff[128] = {0};
    sprintf(buff, "hello_%d", i);
    printf("send  buff=%s\n", buff);
    int ret = zmq_send(requester, buff, strlen(buff), ZMQ_DONTWAIT);
    printf("zmq_send size=%d\n", ret);
    sleep(1);
    i++;
  }
  //  int request_nbr;
  //  for (request_nbr = 0; request_nbr != 10; request_nbr++) {
  //    char buffer[10];
  //    printf("Sending Hello %d…\n", request_nbr);
  //    zmq_send(requester, "Hello", 5, ZMQ_DONTWAIT);
  //    zmq_recv(requester, buffer, 10, 0);
  //    printf("Received World %d\n", request_nbr);
  //  }

  zmq_close(requester);
  zmq_ctx_destroy(context);

  return 0;
}
