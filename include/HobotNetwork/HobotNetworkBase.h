/*
 * HobotNetworkBase.h
 *
 *  Created on: Mar 7, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_
#define ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_
#include <stddef.h>
#include <mutex>
namespace Modo {
typedef void (*MQCallBack)(void *data, void *hint);
enum CON_STATUS { CONNECT_SUCCESS, CONNECT_FAIED };
class HobotNetworkBase {
 public:
  HobotNetworkBase()
      : m_context(nullptr),
        m_requester(nullptr),
        m_buff(nullptr),
        m_buff_size(0),
        m_con_status_(CONNECT_SUCCESS) {}
  virtual ~HobotNetworkBase() {}
  virtual int Init(const char *config) = 0;
  virtual int SendData(const void *data = nullptr, size_t datalen = 0,
                       int timeout = 0);
  virtual int RecvData(void *buff = nullptr, size_t bufflen = 0,
                       int timeout = -1);
  virtual int CopyRecvData(void *buff, size_t bufflen);
  virtual void Finish();
  virtual int NewRequester(const char *config) = 0;
  virtual void DestroyRequester() = 0;
  friend void StartMonitor(void *args);
  virtual void SetConStatus(CON_STATUS status) { m_con_status_ = status; }

 protected:
  void *m_context;
  void *m_requester;
  void *m_monitor;
  void *m_buff;
  size_t m_buff_size;
  CON_STATUS m_con_status_;
  mutable std::mutex m_send_mtx_;
  mutable std::mutex m_recv_mtx_;
};

// this is a monitor for communication
struct MonitorArgs {
  HobotNetworkBase *client;
  const char *config;
};

void StartMonitor(void *args);
}  // namespace Modo


#endif /* ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_ */
