/*
 * HobotZmqServer.cc
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */


#include "HobotNetwork/HobotZmqServer.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include "base/base.h"
namespace Modo {
int HobotZmqServer::Init(const char *config) {
  LOGD << "config= " << config;
  if (!config) {
    LOGD << "config null ";
    return 1;
  }

  m_context = zmq_ctx_new();
  if (!m_context) {
    LOGD << "zmq_ctx_new failed ";
    return 1;
  }

  m_requester = zmq_socket(m_context, ZMQ_DEALER);
  if (!m_requester) {
    LOGD << "zmq_socket failed ";
    return 1;
  }

  int zmq_sndhwm = 1;
  int zmq_sndbuf = 50 * 1024 * 1024;

  zmq_setsockopt(m_requester, ZMQ_SNDHWM, &zmq_sndhwm, sizeof(int));
  zmq_setsockopt(m_requester, ZMQ_SNDBUF, &zmq_sndbuf, sizeof(int));
  int zmq_linger = 1000;
  zmq_setsockopt(m_requester, ZMQ_LINGER, &zmq_linger, sizeof(int));
  int zmq_snd_to = 500;
  zmq_setsockopt(m_requester, ZMQ_SNDTIMEO, &zmq_snd_to, sizeof(int));

  int con_ret = zmq_bind(m_requester, config);
  if (con_ret == -1) {
    LOGD << "zmq_bind failed ";
    return 1;
  }

  MonitorArgs args;
  args.config = config;
  args.client = this;
  // zmq_threadstart(StartMonitor, (void *)&args);

  m_buff_size = 2 * 1024 * 1024;
  m_buff = malloc(m_buff_size);
  return 0;
}
}  // namespace Modo
