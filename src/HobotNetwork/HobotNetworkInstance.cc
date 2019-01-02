#include "HobotNetwork/HobotNetworkInstance.h"
#include "HobotNetwork/HobotZmqServer.h"
#include "HobotNetwork/HobotZmqClient.h"
#include <stdio.h>

HobotClientBase* HobotNetworkInstance::CreateClientInstance(HobotNetworkType type) {
    HobotClientBase* client_instance;
    if(type == HOBOT_ZMQ) {
        // in stack
        client_instance = new HobotZmqClient();    
    }
    return client_instance;
}

HobotServerBase* HobotNetworkInstance::CreateServerInstance(HobotNetworkType type) {
    HobotServerBase* server_instance;
    if(type == HOBOT_ZMQ) {
        server_instance = new HobotZmqServer();    
    }
    return server_instance;
}

void HobotNetworkInstance::DestroyClientInstance(HobotClientBase *client_instance) {
    delete client_instance;
}

void HobotNetworkInstance::DestroyServerInstance(HobotServerBase *server_instance) {
    delete server_instance;
}