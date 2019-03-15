/*
 * HobotZmqClient.cc
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */
#include "HobotNetwork/HobotZmqClient.h"
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hobotlog/hobotlog.hpp"

namespace Modo {

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

int HobotZmqClient::NewRequester(const char *config) {
  // m_context = zmq_ctx_new();
  // if (!m_context) {
  //   LOGE << "zmq_ctx_new failed ";
  //   printf("2\n");
  //   return 1;
  // }

  m_requester = zmq_socket(m_context, ZMQ_DEALER);
  if (!m_requester) {
    LOGE << "zmq_socket failed ";
    printf("3\n");
    return 1;
  }

  int zmq_sndhwm = 1;
  zmq_setsockopt(m_requester, ZMQ_SNDHWM, &zmq_sndhwm, sizeof(int));

  int rc = zmq_connect(m_requester, config);
  assert(rc == 0);

  return 0;
}

void HobotZmqClient::DestroyRequester() {
  zmq_close(m_requester);
  // zmq_ctx_destroy(m_context);
}

void *StartMonitor(void *args) {
  MonitorArgs *arg = (MonitorArgs *)args;
  HobotZmqClient *client = arg->client;
  const char *config = arg->config;

  printf("zmq_socket_monitor begin\n");
  zmq_socket_monitor(client->m_requester, "inproc://monitor-client",
                     ZMQ_EVENT_ALL);
  printf("zmq_socket_monitor end\n");
  client->m_monitor = zmq_socket(client->m_context, ZMQ_PAIR);
  assert(client->m_monitor);
  int rc = zmq_connect(client->m_monitor, "inproc://monitor-client");
  assert(rc == 0);

  int event = 0;
  int timer = 0;
  while (true) {
    if (timer < 10) {
      event = get_monitor_event(client->m_monitor, NULL, NULL);
      if (event == ZMQ_EVENT_CONNECTED) {  // connection success
        break;
      } else {
        timer++;
        usleep(100000);
      }
    } else {
      client->DestroyRequester();
      client->NewRequester(config);
      timer = 0;
    }
  }
  return NULL;
}

int HobotZmqClient::Init(const char *config) {
  if (!config) {
    LOGE << "config null ";
    printf("1\n");
    return 1;
  }
  LOGD << "HobotZmqClient Init config= " << config;

  m_context = zmq_ctx_new();
  if (!m_context) {
    LOGE << "zmq_ctx_new failed ";
    printf("2\n");
    return 1;
  }

  int ret = NewRequester(config);
  if (ret) {
    LOGE << "new requester fail";
  }

  // monitor the connection status
  MonitorArgs args;
  args.config = config;
  args.client = this;
  int rc = pthread_create(&m_thread, NULL, StartMonitor, (void *)&args);
  assert(rc == 0);

  m_buff_size = 2 * 1024 * 1024;
  m_buff = malloc(m_buff_size);
  return 0;
}

void HobotZmqClient::Finish() {
  // zmq_threadclose(thread_);
  HobotNetworkBase::Finish();
}

}  // namespace Modo
