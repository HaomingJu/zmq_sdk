/*
 * DispatchModule.cc
 *
 *  Created on: Jan 15, 2019
 *      Author: user
 */

#include "DispatchModule.h"
#include "hobotlog/hobotlog.hpp"
#include "message/BuffMsg.h"
#include "HobotProtocol/HobotProtocolRead.h"

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
  TransferVector msgvec;
  int8_t* buff_rec = sp_receive_msg->GetBuff();
  int bufflen_rec = sp_receive_msg->GetBuffSize();
  HobotProtocolRead reader(buff_rec, bufflen_rec);
  int32_t length = 0;
  int32_t version = 0;
  int64_t stamp = 0;
  int64_t seq = 0;
  reader.ReadHead(length, version, stamp, seq);
  int type_rec;
  int8_t* data_rec;
  int datalen_rec;
  /* header is 24 bytes and has been read, so we should minus 24 */
  while(length - 24 > 0) {
    struct DataTransferInputMsg msg;
    reader.ReadTLV(type_rec, datalen_rec, &data_rec);
    msg.type = (MsgType)type_rec;
    msg.data = data_rec;
    msg.datalen = datalen_rec;
    msgvec.push_back(msg);
    length -= datalen_rec;
  }
  user_callback_(msgvec);
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
