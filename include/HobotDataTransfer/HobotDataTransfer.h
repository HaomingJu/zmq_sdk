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
/*
 * Message type
 */

// enum MsgType {
//   MSG_JPEG_PREVIEW,        // byte array
//   MSG_JPEG,                // byte array
//   MSG_POROTO,              // byte array
//   MSG_CTL_CAPTURE,         // no data
//   MSG_CTL_PREVIEW_DRIVER,  // no data
//   MSG_CTL_PREVIEW_FULL,    // no data
//   MSG_AUDIO,               // byte array
//   MSG_AUDIO_TIME_STAMP,    // int64
//   MSG_VAD,                 // int32
//   MSG_LIP_COMMAND          // byte array
// };

/*
 * DataTransferInputMsg
 */
struct DataTransferInputMsg {
  int type;
  void *data;
  int datalen;
};

class SendModule;
class ReceiveModule;
class DispatchModule;
class SendMsg;
typedef std::vector<DataTransferInputMsg> TransferVector;
typedef std::function<int(TransferVector &)> TransferCallBack;
typedef std::shared_ptr<SendMsg> spSendMsg;
/*
 * service type
 */
enum SericeType { TRANSFER_SEVER, TRANSFER_CLINET };

class HobotNetworkBase;

/*
 * Data Transfer Class
 */
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
  /****************************************************************************
   * info   : init
   * param  : ip : tcp://192.168.1.20:5555,It is Server Ip.
   *          type : SericeType
   * return : 0 success 1 fail
   ***************************************************************************/
  int Init(const char *ip, SericeType type);

  /****************************************************************************
   * info   : Send
   * param  : msgs : TransferVector
   * return : 0 success 1 fail
   ***************************************************************************/
  int Send(TransferVector &msgs, bool sync = false);

  /****************************************************************************
   * info   : Send
   * param  : type : int
   *          data : data ptr
   *          datalen: data length
   * return : 0 success 1 fail
   ***************************************************************************/
  int Send(int type, void *data = nullptr, int datalen = 0, bool sync = false);

  /****************************************************************************
   * info   : SetReceiveCallback ,will be call when receive data
   * param  : func : TransferCallBack
   *          data : data ptr
   *          datalen: data length
   * return : 0 success 1 fail
   ***************************************************************************/
  void SetReceiveCallback(TransferCallBack func);

  /****************************************************************************
   * info   : StartReceive thread
   * param  :
   * return :
   ***************************************************************************/
  void StartReceive();

  /****************************************************************************
   * info   : Synchronous Receive
   * param  : msgvec : msgs will recv
   * return :
   ***************************************************************************/
  int Receive(TransferVector &msgvec);

  /****************************************************************************
   * info   : Finish
   * param  :
   * return :
   ***************************************************************************/
  void Finish();

  bool GetIsEdianDiff() { return IsEdianDiff_; }

  static void Swap16(int16_t &value);

  static void Swap32(int32_t &value);

  static void Swap64(int64_t &value);

 protected:
  int InitWorkflow();
  int InitNetWork(const char *ip, SericeType type);
  void ExecuteOnThread();
  int DoSend(spSendMsg sp_send_msg, bool sync);

 private:
  bool IsEdianDiff_;
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
