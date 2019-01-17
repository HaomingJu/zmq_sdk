/*
 * DispatchModule.cc
 *
 *  Created on: Jan 15, 2019
 *      Author: user
 */

#include "DispatchModule.h"
#include "hobotlog/hobotlog.hpp"
#include "message/BuffMsg.h"
namespace Modo {
FORWARD_DEFINE(DispatchModule, 0) {
  // LOGD << "ReceiveModule::DoForward";
  DoForward(input, workflow, context, "DispatchModule");
}
void DispatchModule::DoForward(const hobot::MessageLists &input,
                               hobot::Workflow *workflow,
                               hobot::spRunContext &context,
                               std::string MODULE_NAME) {
  LOGD << "DispatchModule::DoForward";
  hobot::MessageList *msg_in = input[0];
  if (!msg_in) {
    LOGE << "slot msg is null ";
    return;
  }

  hobot::spMessage spMsg = msg_in->at(0);
  if (!spMsg) {
    LOGD << "slot msg is null ";
    return;
  }
  spReceiveMsg sp_receive_msg = std::dynamic_pointer_cast<ReceiveMsg>(spMsg);
  // // todo: 完善接收流程：解析接收到的数据->调用用户回调
  //  TransferVector vector;
  //  while(){
  //	  struct DataTransferInputMsg msg;
  //	  msg.type = type;
  //	  msg.data = data;
  //	  msg.datalen = datalen;
  //	  vector.push_back(msg);
  //  }
  //  user_callback_(vector);
  workflow->Return(this, 0, hobot::spMessage(), context);
}
int DispatchModule::Init(hobot::RunContext *context) {
  LOGD << "Init";
  ReceiveBuffMsgPool::Create(5, 10);
  return 0;
}
void DispatchModule::Reset() {
  LOGD << "Reset";
  return;
}
}
