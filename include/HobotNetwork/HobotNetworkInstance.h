/*
 * HobotNetworkInstance.h
 *
 *  Created on: Dec 24, 2018
 *      Author: user
 */

#ifndef HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTNETWORKINSTANCE_H_
#define HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTNETWORKINSTANCE_H_

#include <zmq.h>


// class HobotClientBase {
//  public:
//   virtual ~HobotClientBase() {}
//   virtual int Init(const char *config) = 0;
//   virtual int SendData(const void *data = nullptr, size_t datalen = 0) = 0;
//   virtual int RecvData(void *buff = nullptr, size_t bufflen = 0) = 0;
//   virtual int CopyRecvData(void *buff, size_t bufflen) = 0;
//   virtual void Finish() = 0;
// };
namespace Modo {
typedef void (*MQCallBack)(void *data, void *hint);

// class HobotServerBase {
//  public:
//   virtual ~HobotServerBase() {}
//   virtual int Init(const char *config) = 0;
//   virtual int SendData(const void *data = nullptr, size_t datalen = 0,
//                        bool sync = true, MQCallBack callback = nullptr) = 0;
//   virtual int RecvData(void *buff = nullptr, size_t bufflen = 0) = 0;
//   virtual int CopyRecvData(void *buff, size_t bufflen) = 0;
//   virtual void Finish() = 0;
// };

class HobotNetworkBase {
public:
  virtual ~HobotNetworkBase() {}
  virtual int Init(const char *config) = 0;
  virtual int SendData(const void *data = nullptr, size_t datalen = 0,
                       bool sync = true, MQCallBack callback = nullptr) = 0;
  virtual int RecvData(void *buff = nullptr, size_t bufflen = 0) = 0;
  virtual int CopyRecvData(void *buff, size_t bufflen) = 0;
  virtual void Finish() = 0;
};



// class HobotNetworkInstance {
//  public:
//   HobotNetworkInstance() {}
//   ~HobotNetworkInstance() {}
//   HobotClientBase *CreateClientInstance(HobotNetworkType type);
//   HobotServerBase *CreateServerInstance(HobotNetworkType type);
//   void DestroyClientInstance(HobotClientBase *client_instance);
//   void DestroyServerInstance(HobotServerBase *server_instance);
// };
enum HobotNetworkType {
  HOBOT_ZMQ   = 0,
  HOBOT_HBIPC = 1,
};

class HobotNetworkInstance {
 public:
  HobotNetworkInstance() {}
  ~HobotNetworkInstance() {}
  HobotNetworkBase *CreateClientInstance(HobotNetworkType type);
  HobotNetworkBase *CreateServerInstance(HobotNetworkType type);
  void DestroyNetworkInstance(HobotNetworkBase *net_instance);
  // void DestroyServerInstance(HobotNetworkBase *server_instance);
};
}
#endif /* HOBOT_DMS_3RD_HOBOTNETWORK_SRC_HOBOTNETWORKINSTANCE_H_ */
