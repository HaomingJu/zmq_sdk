/*
 * testReceiveMatrix.cc
 *
 *  Created on: Mar 17, 2019
 *      Author: user
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include "opencv2/opencv.hpp"
//#include <string>
#include "DMSSDKOutput.pb.h"
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "TimeSync/TimeSync.h"
#include "base/base.h"
#include "protocol.h"
#include "statusRenderHelper.h"

int client_callback(Modo::TransferVector &tran_vector) {
  printf("server_call \n");
  long time_start = Modo::GetSysStamp();
  int size = tran_vector.size();
  printf(">>>>>>>>>>>>>>>size = %d \n", size);
  for (int i = 0; i < size; i++) {
    Modo::DataTransferInputMsg msg = tran_vector[i];
    printf("server_call[%d,%d] \n", msg.type, msg.datalen);
    if (msg.type == Modo::MSG_PERCEPTION_POROTO) {
      DMSOutputProtocol::PerceptResult *p =
          new DMSOutputProtocol::PerceptResult();
      p->ParseFromArray(msg.data, msg.datalen);
      StatusRenderHelper help(*p);
      std::vector<std::string> &result = help.getResult();
      int size = result.size();
      printf("size = %d\n", size);
      for (int i = 0; i < size; i++) {
        printf("%s\n", result[i].c_str());
      }
    }
#if 1
    if (msg.type == Modo::MSG_PERCEPTION_JPEG_PREVIEW_DRIVER) {
      printf("beging imshow\n");
      std::vector<char> mData((char *)msg.data,
                              (char *)(msg.data + msg.datalen));
      cv::Mat matRaw = cv::imdecode(mData, cv::IMREAD_COLOR);
      // cv::namedWindow("test",cv::WINDOW_AUTOSIZE);
      cv::imshow("test", matRaw);
      // cv::imwrite("./test.jpeg",matRaw);
      cv::waitKey(1);
    }
#endif
  }

  long span = (Modo::GetSysStamp() - time_start);

  printf(">>>>>>>>>>>>>>>span = %ld \n", span);
  return 0;
}

int main(int argc, char **argv) {
  std::string ip;
  std::string port;
  std::string ipstr;
  if (argc < 3) {
    ipstr = "tcp://localhost:5555";
  } else {
    ip = argv[1];
    port = argv[2];
    ipstr = "tcp://" + ip + ":" + port;
  }

  printf("Connecting to hello world server:%s\n", ipstr.c_str());
  Modo::HobotDataTransfer *transfer = new Modo::HobotDataTransfer();
  if (!transfer) {
    printf("New HobotDataTransfer failed !\n");
    return 1;
  }
  int ret = transfer->Init(ipstr.c_str(), Modo::TRANSFER_CLINET);
  if (ret) {
    printf("HobotDataTransfer Init failed !\n");
    return 1;
  }
  transfer->SetReceiveCallback(client_callback);
  transfer->StartReceive();
  while (1) {
    //	Modo::TransferVector msgvec;
    //	printf("begin Receive\n");
    //	int ret = transfer->Receive(msgvec,1000);
    //	printf("ret=%d\n",ret);
    sleep(1);
    //	break;
  }
  transfer->Finish();
  return 0;
}
