/*
 * BuffMsg.h
 *
 *  Created on: Jan 14, 2019
 *      Author: user
 */

#ifndef ZMQ_SDK_SRC_HOBOTDATATRANSFER_MESSAGE_BUFFMSG_H_
#define ZMQ_SDK_SRC_HOBOTDATATRANSFER_MESSAGE_BUFFMSG_H_
#include <stdint.h>

#include "mempool.h"
namespace Modo {
#define BUFF_SIZE 1024 * 1024

class BuffMsg : public BaseMsg {
 public:
  // todo:miaowangqian
  int8_t *GetBuff() { return buff_.data(); }
  int GetBuffSize() { return buff_.size(); }
  void ResizeBuff(int size) { buff_.resize(size); }

 protected:
  BuffMsg() { buff_.resize(BUFF_SIZE); }
  void Reset() {}

  bool DeepCopy(const BaseMsg *other) { return true; }

 private:
  std::vector<int8_t> buff_;
};
class SendMsg : public BuffMsg {
  friend class MemPool<SendMsg>;
};
class ReceiveMsg : public BuffMsg {
  friend class MemPool<ReceiveMsg>;
};
typedef MemPool<SendMsg> SendBuffMsgPool;
typedef MemPool<ReceiveMsg> ReceiveBuffMsgPool;
typedef std::shared_ptr<SendMsg> spSendMsg;
typedef std::shared_ptr<ReceiveMsg> spReceiveMsg;
}  // end of namespace HobotDMS


#endif /* ZMQ_SDK_SRC_HOBOTDATATRANSFER_MESSAGE_BUFFMSG_H_ */
