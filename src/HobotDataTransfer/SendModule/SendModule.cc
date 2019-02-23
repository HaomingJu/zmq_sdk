/*
 * SendModule.cc
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

//#include <hobot/hobot.h>
#include "../../HobotDataTransfer/SendModule/SendModule.h"
#include "../message/BuffMsg.h"
#include "HobotNetwork/HobotNetworkInstance.h"
#include "hobotlog/hobotlog.hpp"

namespace Modo {
FORWARD_DEFINE(SendModule, 0) {
  // LOGD << "SendModule::DoForward";
  DoForward(input, workflow, context, "SendModule");
}
void SendModule::DoForward(const hobot::MessageLists &input,
                           hobot::Workflow *workflow,
                           hobot::spRunContext &context,
                           std::string MODULE_NAME) {
  LOGD << "SendModule::DoForward";
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
  spSendMsg sp_send_msg = std::dynamic_pointer_cast<SendMsg>(spMsg);
  LOGE << "DO SendModule DoForward ";
  // todo: 完善发送流程：取出消息-->调用通信接口发送
  void *data = sp_send_msg->GetBuff();
  int datalen = sp_send_msg->GetDataSize();
  sender_->SendData(data, datalen);

  workflow->Return(this, 0, hobot::spMessage(), context);
}
int SendModule::Init(hobot::RunContext *context) {
  if (initd_)
    return 0;
  initd_ = true;
  LOGD << "Init";
  return 0;
}
void SendModule::Reset() {
  LOGD << "Reset";
  return;
}
}
