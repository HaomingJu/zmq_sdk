#include "HobotNetwork/HobotNetworkInstance.h"
#include "HobotNetwork/HobotZmqServer.h"
#include "HobotNetwork/HobotZmqClient.h"
#include <stdio.h>

namespace Modo{
HobotNetworkBase* HobotNetworkInstance::CreateClientInstance(HobotNetworkType type) {
    HobotNetworkBase* client;
    if(type == HOBOT_ZMQ) {
        client = new HobotZmqClient();
    }
    return client;
}

HobotNetworkBase* HobotNetworkInstance::CreateServerInstance(HobotNetworkType type) {
    HobotNetworkBase* server;
    if(type == HOBOT_ZMQ) {
        server = new HobotZmqServer();
    }
    return server;
}
// HobotNetworkBase* HobotNetworkInstance::CreateClientInstance(HobotNetworkType type) {
//     HobotNetworkBase* client_instance;
//     if(type == HOBOT_ZMQ) {
//         client_instance = new HobotZmqClient();    
//     }
//     return client_instance;
// }

// HobotNerworkBase* HobotNetworkInstance::CreateServerInstance(HobotNetworkType type) {
//     HobotNetworkBase* server_instance;
//     if(type == HOBOT_ZMQ) {
//         server_instance = new HobotZmqServer();    
//     }
//     return server_instance;
// }

void HobotNetworkInstance::DestroyNetworkInstance(HobotNetworkBase* net_instance) {
    delete net_instance;
}
}