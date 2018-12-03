/*
 * HobotZmqServer.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_

//#include "HobotServerBase.h"
typedef void (*MQCallBack)(void *data, void *hint);
class HobotZmqServer {
 public:
  HobotZmqServer() : m_requester(nullptr) {}
  int Init(const char *config);
  int SendData(void *data, int datalen, MQCallBack callback);
  void Finish(){};

 private:
  void *m_requester;
};

#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_ */
