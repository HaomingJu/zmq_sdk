/*
 * HobotZmqClient.cc
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */
#include "HobotNetwork/HobotZmqClient.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hobotlog/hobotlog.hpp"

namespace Modo {
int HobotZmqClient::Init(const char *config) {
  SetLogLevel(HOBOT_LOG_DEBUG);
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

  /* you can set the socket here, plz read the zmq manual. */
  // int recvhwm = 5;
  // zmq_setsockopt(m_requester, ZMQ_REQ, "", 0);
  // zmq_setsockopt(m_requester, ZMQ_RCVHWM, &recvhwm, sizeof(int));
  // int recv_to;
  // zmq_setsockopt(m_requester, ZMQ_RCVTIMEO, &recv_to, sizeof(int));
  // assert(rc == 0);
  m_buff_size = 2 * 1024 * 1024;
  m_buff = malloc(m_buff_size);
  return 0;
}

int HobotZmqClient::RecvData(void *buff, size_t bufflen) {
  SetLogLevel(HOBOT_LOG_DEBUG);
  int recv_size = 0;
  if (buff != nullptr) {
    recv_size = zmq_recv(m_requester, buff, bufflen, 0);
    if (recv_size == -1) {
      LOGD << "zmq_rcv_data failed in Client";
    }
  } else {
    recv_size = zmq_recv(m_requester, m_buff, m_buff_size, 0);
    if (recv_size == -1) {
      LOGD << "zmq_rcv_data failed in Client";
    } else if (bufflen && m_buff_size > 0) {
      int len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
      memcpy(buff, m_buff, len);
    }
  }
  return recv_size;
}


int HobotZmqClient::CopyRecvData(void *buff, size_t bufflen) {
  assert(buff == nullptr);
  int len = 0;
  if (bufflen && m_buff_size > 0) {
    len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
    memcpy(buff, m_buff, len);
  }
  return len;
}

int HobotZmqClient::SendData(const void *data, size_t datalen, bool sync,
                             MQCallBack callback) {
  if (!data || datalen <= 0) {
    LOGD << "data null";
    return -1;
  }
  int send_size = 0;
  bool tryAgain = true;
  while (tryAgain) {
    send_size = zmq_send(m_requester, data, datalen, 0);
    if (send_size >= 0) {
      tryAgain = false;
    } else {
      if (errno == EAGAIN || errno == EINTR) {
#ifdef _WIN32
        Sleep(10);
#else
        usleep(10000);
#endif
      } else {
        LOGD << "ZMQ Get Error :" << strerror(errno);
        return -1;
      }
    }
  }
  return send_size;
}

void HobotZmqClient::Finish() {
  zmq_close(m_requester);
  zmq_ctx_destroy(m_context);
  free(m_buff);
}
}
