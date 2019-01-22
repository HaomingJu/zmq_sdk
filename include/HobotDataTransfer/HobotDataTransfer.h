/*
 * HobotDataTransfer.h
 *
 *  Created on: Jan 13, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTDATATRANSFER_MAIN_HOBOTDATATRANSFER_H_
#define ZMQ_SDK_SRC_HOBOTDATATRANSFER_MAIN_HOBOTDATATRANSFER_H_

#include <functional>
#include <memory>
#include <vector>
// #include "HobotNetwork/HobotNetworkInstance.h"

namespace hobot {
class HobotNetworkBase;
class Engine;
class Workflow;
class RunContext;
typedef std::shared_ptr<RunContext> spRunContext;
class InputModule;
class Expression;
class RunObserver;
class Module;
}
namespace Modo {
enum MsgType {
  MSG_JPEG,    // byte array
  MSG_POROTO,  // byte array
  MSG_ASR,     // byte array
  MSG_CTL,
  MSG_AUDIO,             // byte array
  MSG_AUDIO_TIME_STAMP,  // int32
  MSG_VAD                // int32
};
struct DataTransferInputMsg {
  MsgType type;
  void *data;
  int datalen;
};
class SendModule;
class ReceiveModule;
class DispatchModule;
typedef std::vector<DataTransferInputMsg> TransferVector;
typedef std::function<int(TransferVector &)> TransferCallBack;
enum SericeType { TRANSFER_SEVER, TRANSFER_CLINET };
class HobotNetworkBase;
class HobotDataTransfer {
 public:
  HobotDataTransfer()
      : inited_(false),
        engine_(nullptr),
        workflow_main_(nullptr),
        trigger_send_(nullptr),
        trigger_receive_(nullptr),
        observer_(nullptr) {}

  ~HobotDataTransfer() {}
  int Init(const char *ip, SericeType type);
  int Send(TransferVector &msgs);
  int Send(MsgType type, void *data, int datalen);
  void SetReceiveCallback(TransferCallBack func);
  void Finish();

 protected:
  int InitWorkflow();
  int InitNetWork(const char *ip, SericeType type);
  void ExecuteOnThread();

 private:
  bool inited_;
  HobotNetworkBase *network_;
  SendModule *send_;
  ReceiveModule *receive_;
  DispatchModule *dispatch_;
  hobot::Engine *engine_;
  hobot::Workflow *workflow_main_;
  hobot::spRunContext workflow_main_rt_ctx_;

  hobot::InputModule *trigger_send_;
  hobot::InputModule *trigger_receive_;
  std::vector<hobot::Module *> modules_;
  std::vector<hobot::Expression *> conditioins_;
  hobot::RunObserver *observer_;
};
}


#endif /* ZMQ_SDK_SRC_HOBOTDATATRANSFER_MAIN_HOBOTDATATRANSFER_H_ */
