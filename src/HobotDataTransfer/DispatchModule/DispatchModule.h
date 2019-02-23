/*
 * DispatchModule.h
 *
 *  Created on: Jan 15, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTDATATRANSFER_DISPATCHMODULE_DISPATCHMODULE_H_
#define ZMQ_SDK_SRC_HOBOTDATATRANSFER_DISPATCHMODULE_DISPATCHMODULE_H_

#include <hobot/hobot.h>
#include <memory>
#include "HobotDataTransfer/HobotDataTransfer.h"
namespace Modo {
class HobotNetworkBase;
class DispatchModule : public hobot::Module {
 public:
  DispatchModule(std::string class_name)
      : hobot::Module("Modo", class_name), inited_(false) {}
  virtual ~DispatchModule() {}

  int Init(hobot::RunContext *context);

  void SetReceiveCallback(TransferCallBack callback) {
    user_callback_ = callback;
  }
  void Reset();
  FORWARD_DECLARE(DispatchModule, 0);

 protected:
  //  std::string GetConfigPathName(hobot::RunContext *context);
  void DoForward(const hobot::MessageLists &input, hobot::Workflow *workflow,
                 hobot::spRunContext &context, std::string MODULE_NAME);
  void SwapData(struct DataTransferInputMsg &msg);

 private:
  TransferCallBack user_callback_;
  bool inited_;
};
}


#endif /* ZMQ_SDK_SRC_HOBOTDATATRANSFER_DISPATCHMODULE_DISPATCHMODULE_H_ */
