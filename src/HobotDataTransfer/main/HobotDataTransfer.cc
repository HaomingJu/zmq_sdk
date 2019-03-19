/*
 * HobotDataTransfer.cc
 *
 *  Created on: Jan 13, 2019
 *      Author: user
 */
#include "HobotDataTransfer/HobotDataTransfer.h"
#include <hobot/hobot.h>
#include <memory.h>
#include "DispatchModule/DispatchModule.h"
#include "HobotNetwork/HobotNetworkBase.h"
#include "HobotNetwork/HobotNetworkInstance.h"
#include "HobotProtocol/HobotProtocolRead.h"
#include "HobotProtocol/HobotProtocolWrite.h"
#include "ReceiveModule/ReceiveModule.h"
#include "SendModule/SendModule.h"
#include "TimeSync/TimeSync.h"
#include "base/base.h"
#include "message/BuffMsg.h"

namespace Modo {
class MutiRunObserver : public hobot::RunObserver {
 public:
  MutiRunObserver() { printf("MutiRunObserver\n"); };
  ~MutiRunObserver(){};

  void OnResult(hobot::Module *from, int forward_index,
                hobot::spMessage output) {
    // printf("onResult\n");
  }
  void OnError(hobot::Module *from, int forward_index,
               hobot::spMessage output) {
    // printf("onError\n");
  }
};


void HobotDataTransfer::Finish() {
  if (!inited_) {
    return;
  }
  LOGD << "begin Finish";
  // stop processing
  workflow_main_->Reset();

  for (size_t i = 0; i < conditioins_.size(); i++) {
    delete conditioins_[i];
  }

  for (size_t i = 0; i < modules_.size(); i++) {
    delete modules_[i];
    modules_[i] = NULL;
  }
  modules_.clear();

  if (trigger_send_)
    delete trigger_send_;
  trigger_send_ = NULL;
  if (trigger_receive_)
    delete trigger_receive_;
  trigger_receive_ = NULL;
  if (observer_)
    delete observer_;
  observer_ = NULL;

  if (workflow_main_)
    delete workflow_main_;
  workflow_main_ = NULL;

  if (engine_)
    delete engine_;
  engine_ = NULL;
  LOGD << "begin DestroyNetworkInstance";
  if (network_) {
    HobotNetworkInstance ins;
    ins.DestroyNetworkInstance(network_);
  }
  network_ = NULL;

  inited_ = false;
}
void HobotDataTransfer::ExecuteOnThread() {
  int tid = 1;
  for (size_t i = 0; i < modules_.size(); i++) {
    for (int j = 0; j < modules_[i]->GetForwardCount(); j++) {
      printf("[HobotDMS][%s] ModuleName[Idx %d][Ford %d] = %s\n", __FUNCTION__,
             i, j, modules_[i]->GetFullClassName().c_str());
      engine_->ExecuteOnThread(modules_[i], j, tid++);
      printf("[%s]-------->Line = %d\n", __FUNCTION__, __LINE__);
    }
  }
}
int HobotDataTransfer::Init(const char *ip, SericeType type) {
  // SetLogLevel(HOBOT_LOG_DEBUG);
  int ret = InitNetWork(ip, type);
  if (ret) {
    LOGE << "InitNetWork  failed !";
    return ret;
  }
  ret = InitWorkflow();
  if (ret) {
    LOGE << "InitWorkflowInitWorkflow  failed !";
    return ret;
  }
  ReceiveBuffMsgPool::Create(10, 50);
  SendBuffMsgPool::Create(10, 50);
  LOGD << "GetEpochTimeMs:" << TimeSync::GetEpochTimeMs();
  return 0;
}

int HobotDataTransfer::InitNetWork(const char *ip, SericeType type) {
  // todo:根据SericeType 创建network实例-->根据ip，初始化network实例
  // todo:network_ =
  HobotNetworkInstance ins;
  switch (type) {
    case TRANSFER_SEVER:
      network_ = ins.CreateServerInstance(HOBOT_ZMQ);
      break;
    case TRANSFER_CLINET:
      network_ = ins.CreateClientInstance(HOBOT_ZMQ);
      break;
  }
  return network_->Init(ip);
  //  WaitPingPong();
  // return 0;
}
typedef std::vector<std::pair<hobot::Module *, int>> ObserverBinding;
int HobotDataTransfer::InitWorkflow() {
  LOGD << "HobotDataTransfer::Init!";
  if (inited_) {
    return 0;
  }
  inited_ = true;

  // g_abs_path = config_file;
  engine_ = hobot::Engine::NewInstance();
  if (!engine_) {
    LOGE << "Engine::NewInstance  failed !";
    return 1;
  }
  workflow_main_ = engine_->NewWorkflow();
  if (!workflow_main_) {
    LOGE << "Engine::NewWorkflow  failed !";
    return 1;
  }
  trigger_send_ = new hobot::InputModule("trigger_module_");
  if (!trigger_send_) {
    LOGE << "trigger_module_ new failed !";
    return 1;
  }
  trigger_receive_ = new hobot::InputModule("input_module_");
  if (!trigger_receive_) {
    LOGE << "trigger_module_ new failed !";
    return 1;
  }

  ObserverBinding oberverbind;

  SendModule *send = new SendModule("SendModule");
  if (!send) {
    LOGE << "SendModule new failed !";
    return 1;
  }
  send->SetSender(network_);
  send_ = send;
  modules_.push_back(send);
  workflow_main_->From(trigger_send_)->To(send);
  oberverbind.push_back(std::make_pair(send, 0));

  ReceiveModule *receive = new ReceiveModule("ReceiveModule");
  if (!receive) {
    LOGD << "ReceiveModule new failed !";
    return 1;
  }
  receive->SetReceiver(network_);
  receive_ = receive;
  modules_.push_back(receive);
  workflow_main_->From(trigger_receive_)->To(receive);
  oberverbind.push_back(std::make_pair(receive, 0));

  DispatchModule *dispatch = new DispatchModule("DispatchModule");
  if (!dispatch) {
    LOGE << "DispatchModule new failed !";
    return 1;
  }
  dispatch_ = dispatch;
  modules_.push_back(dispatch);
  workflow_main_->From(receive)->To(dispatch);
  oberverbind.push_back(std::make_pair(dispatch, 0));
  LOGD << "ExecuteOnThread begin !";


  for (size_t i = 0; i < modules_.size(); i++) {
    hobot::RunContext *context = nullptr;
    modules_[i]->Init(context);
  }
  ExecuteOnThread();

  MutiRunObserver *observer = new MutiRunObserver();
  std::shared_ptr<hobot::Message> spRequest;
  workflow_main_rt_ctx_ = workflow_main_->Run(oberverbind, observer);
  workflow_main_rt_ctx_->Init();

  return 0;
}
void HobotDataTransfer::StartReceive() {
  workflow_main_->Feed(workflow_main_rt_ctx_, trigger_receive_, 0,
                       hobot::spMessage());
}
int HobotDataTransfer::DoSend(spSendMsg sp_send_msg, int timeout, bool sync) {
  if (sync == true) {
    void *data = sp_send_msg->GetBuff();
    int datalen = sp_send_msg->GetDataSize();
    int ret = network_->SendData(data, datalen, timeout);
    if (ret < 0)
      return ret;
    return 0;
  } else {
    workflow_main_->Feed(workflow_main_rt_ctx_, send_, 0, sp_send_msg);
  }
  return 0;
}
int HobotDataTransfer::Send(TransferVector &msgs, int timeout, bool sync) {
  spSendMsg sp_send_msg = SendBuffMsgPool::GetSharedPtrEx(true);

  // todo:  完善组包流程：取出传入msg-->组包-->发送到Sender模块

  if (!sp_send_msg) {
    LOGD << "new  SendMsg failed";
    return TRANSFER_PTR_NULL;
  }

  int8_t *buff = sp_send_msg->GetBuff();
  int bufflen = sp_send_msg->GetBuffSize();
  HobotProtocolWrite writer(buff, bufflen);
  int length = 0;
  writer.WriteHead(0, 0, 0);
  for (auto &msg : msgs) {
    LOGD << "HobotDataTransfer::WriteTLV[" << msg.type << "," << msg.datalen
         << "]";
    int ret = writer.WriteTLV(msg.type, msg.datalen, (int8_t *)msg.data);
    if (ret)
      return ret;
  }
  sp_send_msg->SetDataSize(writer.GetPackageLength());
  LOGD << "HobotDataTransfer::Send length=" << writer.GetPackageLength();
  return DoSend(sp_send_msg, timeout, sync);
  // workflow_main_->Feed(workflow_main_rt_ctx_, send_, 0, sp_send_msg);
  // return 0;
}
int HobotDataTransfer::Send(int type, void *data, int datalen, int timeout,
                            bool sync) {
  spSendMsg sp_send_msg = SendBuffMsgPool::GetSharedPtrEx(true);

  // todo:  完善组包流程：取出传入msg-->组包-->发送到Sender模块

  if (!sp_send_msg) {
    LOGD << "new  SendMsg failed";
    return TRANSFER_PTR_NULL;
  }

  LOGD << "HobotDataTransfer::Send";

  int8_t *buff = sp_send_msg->GetBuff();
  int bufflen = sp_send_msg->GetBuffSize();
  HobotProtocolWrite writer(buff, bufflen);
  // int len_snd = 0;
  /* take the `place` of header */
  writer.WriteHead(0, 0, 0);
  LOGD << "HobotDataTransfer::WriteTLV[" << type << "," << datalen << ","
       << (char *)data << "]";
  int ret = writer.WriteTLV(type, datalen, (int8_t *)data);
  if (ret)
    return ret;
  LOGD << "HobotDataTransfer::length=" << writer.GetPackageLength();
  sp_send_msg->SetDataSize(writer.GetPackageLength());
  return DoSend(sp_send_msg, timeout, sync);
  // workflow_main_->Feed(workflow_main_rt_ctx_, send_, 0, sp_send_msg);
  // return 0;
}
void HobotDataTransfer::SetReceiveCallback(TransferCallBack func) {
  dispatch_->SetReceiveCallback(func);
}

int HobotDataTransfer::Receive(TransferVector &msgvec, int timeout) {
  spReceiveMsg sp_receive_msg = ReceiveBuffMsgPool::GetSharedPtrEx(true);
  if (!sp_receive_msg) {
    return TRANSFER_PTR_NULL;
  }

  void *data = sp_receive_msg->GetBuff();
  int datalen = sp_receive_msg->GetBuffSize();
  LOGD << "do RecvData begin";
  int ret = network_->RecvData(data, datalen, timeout);
  LOGD << "do RecvData end,ret = " << ret;

  if (ret < 0) {
    return ret;
  }

  HobotProtocolRead reader((int8_t *)data, datalen);
  IsEdianDiff_ = reader.GetIsEdianDiff();
  LOGD << "IsEdianDiff_= " << IsEdianDiff_;
  while (true) {
    struct DataTransferInputMsg msg;
    int type_rec = 0;
    int8_t *data_rec = nullptr;
    int datalen_rec = 0;
    int ret = reader.ReadTLV(type_rec, datalen_rec, &data_rec);
    if (ret) {
      return ret;
    }
    msg.type = type_rec;
    msg.data = data_rec;
    msg.datalen = datalen_rec;
    LOGD << "ReadTLV[" << msg.type << "," << msg.datalen << ","
         << (char *)msg.data << "]";
    msgvec.push_back(msg);
  }
  return 0;
}
bool HobotDataTransfer::GetIsEdianDiff() {
  LOGD << "GetIsEdianDiff[" << IsEdianDiff_ << ","
       << dispatch_->GetIsEdianDiff() << "]";
  if (IsEdianDiff_ || dispatch_->GetIsEdianDiff()) {
    return true;
  }
  return false;
}
void HobotDataTransfer::Swap16(int16_t &value) {
  value = ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

void HobotDataTransfer::Swap32(int32_t &value) {
  value = ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
          ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

void HobotDataTransfer::Swap64(int64_t &value) {
  value = ((value & 0x00000000000000FF) << 56) |
          ((value & 0x000000000000FF00) << 40) |
          ((value & 0x0000000000FF0000) << 24) |
          ((value & 0x00000000FF000000) << 8) |
          ((value & 0x000000FF00000000) >> 8) |
          ((value & 0x0000FF0000000000) >> 24) |
          ((value & 0x00FF000000000000) >> 40) |
          ((value & 0xFF00000000000000) >> 56);
}
}
