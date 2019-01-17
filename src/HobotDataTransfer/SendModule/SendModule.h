/*
 * SendModule.h
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTDATATRANSFER_SENDMODULE_SENDMODULE_H_
#define ZMQ_SDK_SRC_HOBOTDATATRANSFER_SENDMODULE_SENDMODULE_H_
#include <hobot/hobot.h>
#include <memory>

namespace Modo {
class HobotNetworkBase;
class SendModule : public hobot::Module {
 public:
  SendModule(std::string class_name) : hobot::Module("Modo", class_name) {}
  virtual ~SendModule() {}

  int Init(hobot::RunContext *context);
  int SetSender(HobotNetworkBase *sender) {
    sender_ = sender;
    return 0;
  }
  void Reset();
  FORWARD_DECLARE(SendModule, 0);

 protected:
  //  std::string GetConfigPathName(hobot::RunContext *context);
  void DoForward(const hobot::MessageLists &input, hobot::Workflow *workflow,
                 hobot::spRunContext &context, std::string MODULE_NAME);

 private:
  HobotNetworkBase *sender_;
};
}


#endif /* ZMQ_SDK_SRC_HOBOTDATATRANSFER_SENDMODULE_SENDMODULE_H_ */
