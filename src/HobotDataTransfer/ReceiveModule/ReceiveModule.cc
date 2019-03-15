/*
 * ReceiveModule.cc
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

//#include <hobot/hobot.h>
#include "ReceiveModule.h"
#include "HobotNetwork/HobotNetworkBase.h"
#include "HobotNetwork/HobotNetworkInstance.h"
#include "base/base.h"
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
  if (sp_receive_msg) {
    // todo: 完善接收流程： 将接收数据放入缓存buff-->发送到dispatch模块
    void *data = sp_receive_msg->GetBuff();
    int datalen = sp_receive_msg->GetBuffSize();
    LOGD << "do RecvData begin";
    int ret = receive_->RecvData(data, datalen, -1);
    LOGD << "do RecvData end,ret = " << ret;
    if (ret > 0) {
      workflow->Return(this, 0, sp_receive_msg, context);
    }
  } else {
    LOGD << "sp_receive_msg new failed!";
  }
  // Reschedule time modify 1ms
  workflow->Reschedule(this, 0, input, context, 0);
}
int ReceiveModule::Init(hobot::RunContext *context) {
  if (initd_)
    return 0;
  initd_ = true;
  LOGD << "Init";
  // ReceiveBuffMsgPool::Create(10, 50);
  return 0;
}
void ReceiveModule::Reset() {
  LOGD << "Reset";
  return;
}
}
