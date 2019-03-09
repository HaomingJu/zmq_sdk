/*
 * HobotZmqClient.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_

#include <zmq.h>
#include "HobotNetworkBase.h"

namespace Modo {
class HobotZmqClient : public HobotNetworkBase {
 public:
  HobotZmqClient() {}
  ~HobotZmqClient() {}
  int Init(const char *config);
  void Finish();

 private:
  void *thread_;
  //  void *m_context;
  //  void *m_requester;
  //  void *m_buff;
  //  size_t m_buff_size;
};
}

#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_ */
