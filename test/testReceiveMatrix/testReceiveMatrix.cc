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
#include <string>
#include "DMSSDKOutput.pb.h"
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "protocol.h"
#include "statusRenderHelper.h"


int client_callback(Modo::TransferVector &tran_vector) {
  printf("server_call \n");
  int size = tran_vector.size();
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
  }
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
    sleep(1);
  }
  transfer->Finish();
  return 0;
}
