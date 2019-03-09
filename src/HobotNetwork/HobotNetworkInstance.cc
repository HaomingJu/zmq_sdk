#include "HobotNetwork/HobotNetworkInstance.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include "HobotNetwork/HobotNetworkBase.h"
#include "HobotNetwork/HobotZmqClient.h"
#include "HobotNetwork/HobotZmqServer.h"
#include "hobotlog/hobotlog.hpp"
namespace Modo {
HobotNetworkBase *HobotNetworkInstance::CreateClientInstance(
    HobotNetworkType type) {
  HobotNetworkBase *client;
  if (type == HOBOT_ZMQ) {
    client = new HobotZmqClient();
  }
  return client;
}

HobotNetworkBase *HobotNetworkInstance::CreateServerInstance(
    HobotNetworkType type) {
  HobotNetworkBase *server;
  if (type == HOBOT_ZMQ) {
    server = new HobotZmqServer();
  }
  return server;
}
// HobotNetworkBase* HobotNetworkInstance::CreateClientInstance(HobotNetworkType
// type) {
//     HobotNetworkBase* client_instance;
//     if(type == HOBOT_ZMQ) {
//         client_instance = new HobotZmqClient();
//     }
//     return client_instance;
// }

// HobotNerworkBase* HobotNetworkInstance::CreateServerInstance(HobotNetworkType
// type) {
//     HobotNetworkBase* server_instance;
//     if(type == HOBOT_ZMQ) {
//         server_instance = new HobotZmqServer();
//     }
//     return server_instance;
// }

void HobotNetworkInstance::DestroyNetworkInstance(
    HobotNetworkBase *net_instance) {
  net_instance->Finish();
  delete net_instance;
}
}
