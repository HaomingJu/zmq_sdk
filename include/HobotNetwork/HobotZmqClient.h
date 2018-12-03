/*
 * HobotZmqClient.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_

#include <zmq.h>

//#include "HobotClientBase.h"
class HobotZmqClient {
 public:
  int Init(const char *config);
  int RecvData(void *buff = nullptr, int bufflen = 0);
  int CopyRecvData(void *buff, int bufflen);
  void Finish(){};

 private:
  void *m_requester;
  zmq_msg_t m_msg;
  int m_recv_size;
};


#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_ */
