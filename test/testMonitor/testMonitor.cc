/*
 * testMonitor.cc
 *
 *  Created on: Mar 8, 2019
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include <string>
static int get_monitor_event_internal(void *monitor, int *value, char **address,
                                      int recv_flag) {
  //  First frame in message contains event number and value
  zmq_msg_t msg;
  zmq_msg_init(&msg);
  if (zmq_msg_recv(&msg, monitor, recv_flag) == -1) {
    assert(errno == EAGAIN);
    return -1;  //  timed out or no message available
  }
  assert(zmq_msg_more(&msg));

  uint8_t *data = (uint8_t *)zmq_msg_data(&msg);
  uint16_t event = *(uint16_t *)(data);
  if (value)
    *value = *(uint32_t *)(data + 2);

  //  Second frame in message contains event address
  zmq_msg_init(&msg);
  int res = zmq_msg_recv(&msg, monitor, recv_flag) == -1;
  assert(res != -1);
  assert(!zmq_msg_more(&msg));

  if (address) {
    uint8_t *data = (uint8_t *)zmq_msg_data(&msg);
    size_t size = zmq_msg_size(&msg);
    *address = (char *)malloc(size + 1);
    memcpy(*address, data, size);
    *address[size] = 0;
  }
  return event;
}
int get_monitor_event_with_timeout(void *monitor, int *value, char **address,
                                   int timeout) {
  int res;
  if (timeout == -1) {
    // process infinite timeout in small steps to allow the user
    // to see some information on the console

    int timeout_step = 250;
    int wait_time = 0;
    zmq_setsockopt(monitor, ZMQ_RCVTIMEO, &timeout_step, sizeof(timeout_step));
    while ((res = get_monitor_event_internal(monitor, value, address, 0)) ==
           -1) {
      wait_time += timeout_step;
      fprintf(stderr, "Still waiting for monitor event after %i ms\n",
              wait_time);
    }
  } else {
    zmq_setsockopt(monitor, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    res = get_monitor_event_internal(monitor, value, address, 0);
  }
  int timeout_infinite = -1;
  zmq_setsockopt(monitor, ZMQ_RCVTIMEO, &timeout_infinite,
                 sizeof(timeout_infinite));
  return res;
}
int get_monitor_event(void *monitor, int *value, char **address) {
  return get_monitor_event_with_timeout(monitor, value, address, -1);
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

  printf("Connecting to hello world server:%s\n", ipstr.c_str());

  /*创建一个新的上下文*/
  void *context = zmq_ctx_new();
  void *client = zmq_socket(context, ZMQ_DEALER);
  printf("zmq_socket_monitor begin\n");
  int rc = zmq_socket_monitor(client, "inproc://monitor-client", ZMQ_EVENT_ALL);
  printf("zmq_socket_monitor end\n");
  void *client_mon = zmq_socket(context, ZMQ_PAIR);
  assert(client_mon);
  printf("client_mon zmq_connect begin\n");
  rc = zmq_connect(client_mon, "inproc://monitor-client");
  assert(rc == 0);
  printf("client_mon zmq_connect end\n");
  /*通过tcp协议，5555端口，连接本机服务端*/

  printf("client zmq_connect begin\n");
  zmq_connect(client, ipstr.c_str());
  printf("client zmq_connect end\n");
  printf("get_monitor_event begin\n");
  int event = 0;
  int pre_event = 0;
  while (true) {
    event = get_monitor_event(client_mon, NULL, NULL);
    if (event != pre_event) {
      printf("event:%d,%d\n", event, ZMQ_EVENT_ALL);
      pre_event = event;
    }
    usleep(100000);
  }


  printf("get_monitor_event end\n");

  zmq_close(client);
  zmq_close(client_mon);
  zmq_ctx_destroy(context);

  return 0;
}
