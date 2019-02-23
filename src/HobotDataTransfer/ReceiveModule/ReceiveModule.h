/*
 * SendModule.h
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTDATATRANSFER_RECEIVEMODULE_RECEIVEMODULE_H_
#define ZMQ_SDK_SRC_HOBOTDATATRANSFER_RECEIVEMODULE_RECEIVEMODULE_H_
#include <hobot/hobot.h>
#include <memory>
#include "HobotNetwork/HobotNetworkInstance.h"
namespace Modo {
class HobotNetworkBase;
class ReceiveModule : public hobot::Module {
 public:
  ReceiveModule(std::string class_name)
      : hobot::Module("Modo", class_name), initd_(false) {}
  virtual ~ReceiveModule() {}

  int Init(hobot::RunContext *context);
  int SetReceiver(HobotNetworkBase *receive) {
    receive_ = receive;
    return 0;
  }
  void Reset();
  FORWARD_DECLARE(ReceiveModule, 0);

 protected:
  //  std::string GetConfigPathName(hobot::RunContext *context);
  void DoForward(const hobot::MessageLists &input, hobot::Workflow *workflow,
                 hobot::spRunContext &context, std::string MODULE_NAME);

 private:
  HobotNetworkBase *receive_;
  int initd_;
};
}


#endif /* ZMQ_SDK_SRC_HOBOTDATATRANSFER_RECEIVEMODULE_RECEIVEMODULE_H_ */
