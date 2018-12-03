/*
 * HobotZmqServer.cc
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */


#include "HobotNetwork/HobotZmqServer.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include "hobotlog/hobotlog.hpp"
int HobotZmqServer::Init(const char *config) {
  if (!config) {
    LOGD << "config null ";
    return 1;
  }
  std::string ip = config;

  void *context = zmq_ctx_new();
  if (!context) {
    LOGD << "zmq_ctx_new failed ";
    return 1;
  }

  m_requester = zmq_socket(context, ZMQ_PUB);
  if (!m_requester) {
    LOGD << "zmq_socket failed ";
    return 1;
  }


  int zmq_sndhwm = 10;  // message
  int zmq_sndbuf = 50 * 1024 * 1024;

  zmq_setsockopt(m_requester, ZMQ_SNDHWM, &zmq_sndhwm, sizeof(int));
  zmq_setsockopt(m_requester, ZMQ_SNDBUF, &zmq_sndbuf, sizeof(int));
  int zmq_linger = 1000;
  zmq_setsockopt(m_requester, ZMQ_LINGER, &zmq_linger, sizeof(int));
  int zmq_snd_to = 500;
  zmq_setsockopt(m_requester, ZMQ_SNDTIMEO, &zmq_snd_to, sizeof(int));

  int con_ret = zmq_bind(m_requester, ip.c_str());
  if (con_ret == -1) {
    LOGD << "zmq_bind failed ";
    return 1;
  }
  return 0;
}
static int SendSingleMsg(void *s, zmq_msg_t &msg, int flag) {
  int rc = 0;

  bool bTryAgain = true;
  while (bTryAgain) {
    rc = zmq_msg_send(&msg, s, flag);
    if (rc >= 0) {
      bTryAgain = false;
    } else {
      if (errno == EAGAIN || errno == EINTR) {
#ifdef _WIN32
        Sleep(10);
#else
        usleep(10000);
#endif
      } else {
#ifdef __ANDROID__
        ALOGE("ZMQ Get Error %s", strerror(errno));
#endif
        LOGD << "ZMQ Get Error :" << strerror(errno);
        return -1;
      }
    }
  }  // end while

  return 0;
}

int HobotZmqServer::SendData(void *data, int datalen, MQCallBack callback) {
  int meta_send_flag = ZMQ_NOBLOCK;
  zmq_msg_t msg;
  int rc = zmq_msg_init_data(&msg, data, datalen, NULL, NULL);
  if (rc != 0) {
    return 0;
  }
  return SendSingleMsg(m_requester, msg, meta_send_flag);
  // return 0;
}
