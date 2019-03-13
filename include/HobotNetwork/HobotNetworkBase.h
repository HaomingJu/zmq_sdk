/*
 * HobotNetworkBase.h
 *
 *  Created on: Mar 7, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_
#define ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_
#include <stddef.h>
namespace Modo {
typedef void (*MQCallBack)(void *data, void *hint);
class HobotNetworkBase {
 public:
  HobotNetworkBase()
      : m_context(nullptr),
        m_requester(nullptr),
        m_buff(nullptr),
        m_buff_size(0) {}
  virtual ~HobotNetworkBase() {}
  virtual int Init(const char *config) = 0;
  virtual int SendData(const void *data = nullptr, size_t datalen = 0,
                       int timeout = 0);
  virtual int RecvData(void *buff = nullptr, size_t bufflen = 0);
  virtual int CopyRecvData(void *buff, size_t bufflen);
  virtual void Finish();

 protected:
  void *m_context;
  void *m_requester;
  void *m_monitor;
  void *m_buff;
  size_t m_buff_size;
};
}  // namespace Modo


#endif /* ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_ */