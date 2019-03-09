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
void HobotZmqClient::Finish() {
  zmq_threadclose(thread_);
  HobotNetworkBase::Finish();
}
}
