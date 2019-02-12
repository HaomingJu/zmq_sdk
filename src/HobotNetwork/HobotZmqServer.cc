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
#include <unistd.h>
#include <zmq.h>
#include "hobotlog/hobotlog.hpp"
namespace Modo {
int HobotZmqServer::Init(const char *config) {
  SetLogLevel(HOBOT_LOG_DEBUG);
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
  m_buff_size = 2 * 1024 * 1024;
  m_buff = malloc(m_buff_size);
  return 0;
}

int HobotZmqServer::SendData(const void *data, size_t datalen, bool sync,
                             MQCallBack callback) {
  if (!data || datalen <= 0) {
    LOGD << "data null";
    return 1;
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

int HobotZmqServer::RecvData(void *buff, size_t bufflen) {
  int recv_size = 0;
  if (buff != nullptr) {
    recv_size = zmq_recv(m_requester, buff, bufflen, 0);
    if (recv_size == -1) {
      LOGD << "zmq_rcv_data failed in Server";
    }
  } else {
    recv_size = zmq_recv(m_requester, m_buff, m_buff_size, 0);
    if (recv_size == -1) {
      LOGD << "zmq_rcv_data failed in Server";
    } else if (bufflen && m_buff_size > 0) {
      int len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
      memcpy(buff, m_buff, len);
    }
  }
  return recv_size;
}

int HobotZmqServer::CopyRecvData(void *buff, size_t bufflen) {
  assert(buff == nullptr);
  int len = 0;
  if (bufflen && m_buff_size > 0) {
    len = (bufflen > m_buff_size) ? m_buff_size : bufflen;
    memcpy(buff, m_buff, len);
  }
  return len;
}

void HobotZmqServer::Finish() {
  zmq_close(m_requester);
  zmq_ctx_destroy(m_context);
  free(m_buff);
}
}
