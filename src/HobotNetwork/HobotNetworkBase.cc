/*
 * HobotNetworkBase.cc
 *
 *  Created on: Mar 7, 2019
 *      Author: user
 */
#include "HobotNetwork/HobotNetworkBase.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include "base/base.h"
namespace Modo {
static int get_monitor_event_internal(void *monitor, int *value, char **address,
                                      int recv_flag) {
  //  First frame in message contains event number and value
  zmq_msg_t msg;
  zmq_msg_init(&msg);
  if (zmq_msg_recv(&msg, monitor, recv_flag) == -1) {
    LOGE << "errno == EAGAIN" << errno;
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
      // fprintf(stderr, "Still waiting for monitor event after %i ms\n",
      //         wait_time);
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

int HobotNetworkBase::RecvData(void *buff, size_t bufflen, int timeout) {
  int recv_size = 0;
  if (m_con_status_ == CONNECT_FAIED) {
    return TRANSFER_UNCONNECT_ERROR;
  }
  std::unique_lock<std::mutex> lck(m_recv_mtx_);
  int rc = zmq_setsockopt(m_requester, ZMQ_RCVTIMEO, &timeout, sizeof(int));
  assert(rc == 0);
  if (buff != nullptr) {
    recv_size = zmq_recv(m_requester, buff, bufflen, 0);
    if (recv_size == -1) {
      // LOGD << "zmq_rcv_data failed in Client";
    }
  } else {
    recv_size = zmq_recv(m_requester, m_buff, m_buff_size, 0);
    if (recv_size == -1) {
      // LOGD << "zmq_rcv_data failed in Client";
    } else if (bufflen && m_buff_size > 0) {
      int len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
      memcpy(buff, m_buff, len);
    }
  }
  if (recv_size < 0)
    return TRANSFER_TIMEOUT;
  return recv_size;
}


int HobotNetworkBase::CopyRecvData(void *buff, size_t bufflen) {
  assert(buff == nullptr);
  int len = 0;
  if (bufflen && m_buff_size > 0) {
    len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
    memcpy(buff, m_buff, len);
  }
  return len;
}

int HobotNetworkBase::SendData(const void *data, size_t datalen, int timeout) {
  if (!data || datalen <= 0) {
    LOGD << "data null";
    return -1;
  }
  if (m_con_status_ == CONNECT_FAIED) {
    return TRANSFER_UNCONNECT_ERROR;
  }
  // printf("ZMQ_SNDTIMEO=%d\n",timeout);
  std::unique_lock<std::mutex> lck(m_send_mtx_);
  int rc = zmq_setsockopt(m_requester, ZMQ_SNDTIMEO, &timeout, sizeof(int));
  assert(rc == 0);
  int send_size = 0;
  bool tryAgain = true;
  send_size = zmq_send(m_requester, data, datalen, 0);

  if (send_size < 0) {
    return TRANSFER_TIMEOUT;
  }
  //  while (tryAgain) {
  //    send_size = zmq_send(m_requester, data, datalen, 0);
  //    if (send_size >= 0) {
  //      tryAgain = false;
  //    } else {
  //      if (errno == EAGAIN || errno == EINTR) {
  //#ifdef _WIN32
  //        Sleep(10);
  //#else
  //        usleep(10000);
  //#endif
  //      } else {
  //        LOGD << "ZMQ Get Error :" << strerror(errno);
  //        return -1;
  //      }
  //    }
  //  }
  return send_size;
}

void HobotNetworkBase::Finish() {
  zmq_close(m_requester);
  zmq_ctx_destroy(m_context);
  free(m_buff);
}

void StartMonitor(void *args) {
  MonitorArgs *arg = (MonitorArgs *)args;
  HobotNetworkBase *client = arg->client;
  const char *config = arg->config;

  printf("zmq_socket_monitor begin\n");
  zmq_socket_monitor(client->m_requester, "inproc://monitor-client",
                     ZMQ_EVENT_ALL);
  printf("zmq_socket_monitor end\n");
  client->m_monitor = zmq_socket(client->m_context, ZMQ_PAIR);
  assert(client->m_monitor);
  printf(" before connect\n");
  int rc = zmq_connect(client->m_monitor, "inproc://monitor-client");
  printf(" after connect\n");
  assert(rc == 0);

  int event = 0;
  int timer = 0;
  while (true) {
    if (timer < 10) {
      event = get_monitor_event(client->m_monitor, NULL, NULL);
      LOGE << " THE EVENT" << event;
      printf("The event is %d\n", event);
      if (event == ZMQ_EVENT_CONNECTED ||
          event == 4096) {  // connection success
        // break;
      } else {
        timer++;
      }
      usleep(100000);
    } else {
      client->DestroyRequester();
      client->NewRequester(config);
      timer = 0;
    }
  }
}
}  // namespace Modo
