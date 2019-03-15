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
int HobotNetworkBase::RecvData(void *buff, size_t bufflen, int timeout) {
  int recv_size = 0;
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
  // printf("ZMQ_SNDTIMEO=%d\n",timeout);
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
}
