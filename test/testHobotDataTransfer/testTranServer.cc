/*
 * testTranSever.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
struct SyncFreatureRequest {
  int32_t camera_id;
  int32_t traking_id;
};
int server_call(Modo::TransferVector &tran_vector) {
  printf("server_call \n");
  int size = tran_vector.size();
  for (int i = 0; i < size; i++) {
    Modo::DataTransferInputMsg &msg = tran_vector[i];
    printf("server_call[%d,%d,%s] \n", msg.type, msg.datalen, (char *)msg.data);
    if (msg.type == 1009) {
      SyncFreatureRequest *sync_request = (SyncFreatureRequest *)msg.data;
      printf("sync_request[%d,%d]\n", sync_request->camera_id,
             sync_request->traking_id);
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  std::string ip;
  std::string port;
  std::string ipstr;
  if (argc < 3) {
    ipstr = "tcp://*:5555";
  } else {
    ip = argv[1];
    port = argv[2];
    ipstr = "tcp://" + ip + ":" + port;
  }
  printf("Listening:%s\n", ipstr.c_str());
  //  Socket to talk to clients
  Modo::HobotDataTransfer transfer;
  int ret = transfer.Init(ipstr.c_str(), Modo::TRANSFER_SEVER);
  if (ret) {
    printf("transfer int failed:%s\n", ipstr.c_str());
    return 1;
  }
  transfer.SetReceiveCallback(server_call);
  transfer.StartReceive();
  int i = 0;
  while (1) {
    char data[128] = {0};
    //    sprintf(data, "server%d", i);
    printf("GetIsEdianDiff :%d\n", transfer.GetIsEdianDiff());
    //    transfer.Send(1, data, strlen(data));
    sleep(1);
    i++;
  }
  transfer.Finish();
  return 0;
}
