/*
 * ReceiveModule.cc
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

//#include <hobot/hobot.h>
#include "ReceiveModule.h"
#include "hobotlog/hobotlog.hpp"
#include "message/BuffMsg.h"
namespace Modo {
FORWARD_DEFINE(ReceiveModule, 0) {
  // LOGD << "ReceiveModule::DoForward";
  DoForward(input, workflow, context, "ReceiveModule");
}
void ReceiveModule::DoForward(const hobot::MessageLists &input,
                              hobot::Workflow *workflow,
                              hobot::spRunContext &context,
                              std::string MODULE_NAME) {
  LOGD << "ReceiveModule::DoForward";

  spReceiveMsg sp_receive_msg = ReceiveBuffMsgPool::GetSharedPtrEx(true);
  // todo: 完善接收流程： 将接收数据放入缓存buff-->发送到dispatch模块

  workflow->Return(this, 0, sp_receive_msg, context);

  // Reschedule time modify 1ms
  workflow->Reschedule(this, 0, input, context, 1000);
}
int ReceiveModule::Init(hobot::RunContext *context) {
  LOGD << "Init";
  ReceiveBuffMsgPool::Create(5, 10);
  return 0;
}
void ReceiveModule::Reset() {
  LOGD << "Reset";
  return;
}
}
