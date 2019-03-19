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
enum OP_TYPE { OP_SEND, OP_RECV };
class HobotNetworkBase {
 public:
  HobotNetworkBase()
      : m_context(nullptr),
        m_requester(nullptr),
        m_buff(nullptr),
        m_buff_size(0),
        m_con_status_(CONNECT_SUCCESS),
        thread_(nullptr) {}
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

 private:
  int TimeoutDealData(void *ptr, size_t len, int timeout, OP_TYPE type);
  int DoRecvData(void *buff, size_t bufflen, int timeout);
  int DoDealData(void *ptr, size_t len, int timeout, OP_TYPE type);
  int DoSendData(const void *data, size_t datalen, int timeout);

 public:
  void *m_context;
  void *m_requester;
  void *m_monitor;

 protected:
  void *m_buff;
  size_t m_buff_size;
  CON_STATUS m_con_status_;
  mutable std::mutex m_send_mtx_;
  mutable std::mutex m_recv_mtx_;
  void *thread_;
};

// this is a monitor for communication
struct MonitorArgs {
  HobotNetworkBase *client;
  const char *config;
  const char *monitor_inproc;
};

void StartMonitor(void *args);
}  // namespace Modo


#endif /* ZMQ_SDK_SRC_HOBOTNETWORK_HOBOTNETWORKBASE_H_ */
