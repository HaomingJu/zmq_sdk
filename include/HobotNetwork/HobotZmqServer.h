/*
 * HobotZmqServer.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_


#include "HobotNetworkInstance.h"
#include <zmq.h>

class HobotZmqServer: public HobotServerBase {
 public:
  HobotZmqServer() : m_context(nullptr), 
  m_requester(nullptr), m_buff(nullptr), m_buff_size(0) {}
  ~HobotZmqServer() {}
  int Init(const char *config);
  int SendData(const void *data = nullptr, size_t datalen = 0,
   bool sync = true, MQCallBack callback = nullptr);
  int RecvData(void *buff = nullptr, size_t bufflen = 0);
  int CopyRecvData(void *buff, size_t bufflen);
  void Finish();

 private:
  void *m_context;
  void *m_requester;
  void *m_buff;
  size_t m_buff_size;
};

#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_ */
