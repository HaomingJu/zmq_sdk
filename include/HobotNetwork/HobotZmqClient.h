/*
 * HobotZmqClient.h
 *
 *  Created on: Nov 29, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_

#include <pthread.h>
#include <zmq.h>
#include "HobotNetworkBase.h"

namespace Modo {
class HobotZmqClient : public HobotNetworkBase {
 public:
  HobotZmqClient() {}
  ~HobotZmqClient() {}
  int Init(const char *config);
  void Finish();
  friend void *StartMonitor(void *args);

 private:
  int NewRequester(const char *config);
  void DestroyRequester();
  pthread_t m_thread;
};

// this is a monitor for communication
struct MonitorArgs {
  HobotZmqClient *client;
  const char *config;
};

void *StartMonitor(void *args);
}  // namespace Modo

#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTZMQCLIENT_H_ */
