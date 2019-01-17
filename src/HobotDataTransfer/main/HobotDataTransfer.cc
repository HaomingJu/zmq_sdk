/*
 * HobotDataTransfer.cc
 *
 *  Created on: Jan 13, 2019
 *      Author: user
 */

#include "HobotDataTransfer/HobotDataTransfer.h"
#include <hobot/hobot.h>
#include "DispatchModule/DispatchModule.h"
#include "ReceiveModule/ReceiveModule.h"
#include "SendModule/SendModule.h"
#include "hobotlog/hobotlog.hpp"
#include "message/BuffMsg.h"
namespace Modo {
class MutiRunObserver : public hobot::RunObserver {
 public:
  MutiRunObserver() { printf("MutiRunObserver\n"); };
  ~MutiRunObserver(){};

  void OnResult(hobot::Module *from, int forward_index,
                hobot::spMessage output) {
    printf("onResult\n");
  }
  void OnError(hobot::Module *from, int forward_index,
               hobot::spMessage output) {
    printf("onError\n");
  }
};


void HobotDataTransfer::Finish() {
  if (!inited_) {
    return;
  }

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
  return 0;
}

int HobotDataTransfer::InitNetWork(const char *ip, SericeType type) {
  // todo:根据SericeType 创建network实例-->根据ip，初始化network实例
  // todo:network_ =
  return 0;
}
typedef std::vector<std::pair<hobot::Module *, int>> ObserverBinding;
int HobotDataTransfer::InitWorkflow() {
  SetLogLevel(HOBOT_LOG_DEBUG);
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
    LOGE << "ReceiveModule new failed !";
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


  SendBuffMsgPool::Create(5, 10);


  workflow_main_->Feed(workflow_main_rt_ctx_, trigger_send_, 0,
                       hobot::spMessage());
  workflow_main_->Feed(workflow_main_rt_ctx_, trigger_receive_, 0,
                       hobot::spMessage());


  return 0;
}

int HobotDataTransfer::Send(TransferVector &msgs) {
  spSendMsg sp_send_msg = SendBuffMsgPool::GetSharedPtrEx(true);

  // todo:  完善组包流程：取出传入msg-->组包-->发送到Sender模块

  if (!sp_send_msg) {
    LOGE << "new  SendMsg failed";
    return 1;
  }
  // buff = sp_send_msg->GetBuff()
  //  while(){
  //   write to buff
  //  }

  LOGD << "HobotDataTransfer::Send";
  workflow_main_->Feed(workflow_main_rt_ctx_, send_, 0, sp_send_msg);
  return 0;
}
int HobotDataTransfer::Send(MsgType type, void *data, int datalen) {
  spSendMsg sp_send_msg = SendBuffMsgPool::GetSharedPtrEx(true);

  // todo:  完善组包流程：取出传入msg-->组包-->发送到Sender模块

  if (!sp_send_msg) {
    LOGE << "new  SendMsg failed";
    return 1;
  }
  // buff = sp_send_msg->GetBuff()
  //
  //   write to buff
  //

  LOGD << "HobotDataTransfer::Send";
  workflow_main_->Feed(workflow_main_rt_ctx_, send_, 0, sp_send_msg);
  return 0;
}
void HobotDataTransfer::SetReceiveCallback(TransferCallBack func) {
  dispatch_->SetReceiveCallback(func);
}
}
