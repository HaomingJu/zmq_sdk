/*
 * HobotZmqClient.cc
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */
#include "HobotNetwork/HobotZmqClient.h"
#include <errno.h>
#include <string.h>
#include "hobotlog/hobotlog.hpp"
int HobotZmqClient::Init(const char *config) {
  if (!config) {
    LOGE << "config null ";
    return 1;
  }

  void *context = zmq_ctx_new();
  if (!context) {
    LOGE << "zmq_ctx_new failed ";
    return 1;
  }

  m_requester = zmq_socket(context, ZMQ_SUB);
  if (!m_requester) {
    LOGE << "zmq_socket failed ";
    return 1;
  }

  int rc = zmq_connect(m_requester, config);
  // assert(rc == 0);
  int recvhwm = 5;
  zmq_setsockopt(m_requester, ZMQ_SUBSCRIBE, "", 0);
  zmq_setsockopt(m_requester, ZMQ_RCVHWM, &recvhwm, sizeof(int));
  int recv_to;
  zmq_setsockopt(m_requester, ZMQ_RCVTIMEO, &recv_to, sizeof(int));
  int bufflen = 2 * 1024 * 1024;
  m_buff = malloc(bufflen);
  rc = zmq_msg_init_data(&m_msg, m_buff, bufflen, nullptr, nullptr);
  /// assert(rc == 0);
  return 0;
}
int HobotZmqClient::RecvData(void *buff, int bufflen) {
  int rc = zmq_msg_recv(&m_msg, m_requester, 0);
  if (rc == -1) {
    LOGE << "recv frame structure failed";
    return -1;
  }

  m_recv_size = zmq_msg_size(&m_msg);
  if (buff && bufflen > 0) {
    int len = (bufflen > m_recv_size ? m_recv_size : bufflen);
    memcpy(buff, zmq_msg_data(&m_msg), len);
  }
  return m_recv_size;
}
int HobotZmqClient::CopyRecvData(void *buff, int bufflen) {
  m_recv_size = zmq_msg_size(&m_msg);
  if (buff && bufflen > 0) {
    int len = (bufflen > m_recv_size ? m_recv_size : bufflen);
    memcpy(buff, zmq_msg_data(&m_msg), len);
  }
  return m_recv_size;
}
