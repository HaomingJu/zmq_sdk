/*
 * HobotZmqServer.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_


#include <zmq.h>
#include "HobotNetworkBase.h"

namespace Modo {
class HobotZmqServer : public HobotNetworkBase {
 public:
  HobotZmqServer() {}
  ~HobotZmqServer() {}
  int Init(const char *config);
  int NewRequester(const char *config){};
  void DestroyRequester(){};
  //
  // private:
  //  void *m_context;
  //  void *m_requester;
  //  void *m_buff;
  //  size_t m_buff_size;
};
}  // namespace Modo
#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQSEVER_H_ */
