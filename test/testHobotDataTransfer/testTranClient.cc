/*
 * testTranClient.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
int client_call(Modo::TransferVector &tran_vector) {
  printf("server_call \n");
  int size = tran_vector.size();
  for (int i = 0; i < size; i++) {
    Modo::DataTransferInputMsg msg = tran_vector[i];
    printf("client_call[%d,%d,%s] \n", msg.type, msg.datalen, (char *)msg.data);
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
  printf("connecting:%s\n", ipstr.c_str());
  //  Socket to talk to clients
  Modo::HobotDataTransfer transfer;

  int ret = transfer.Init(ipstr.c_str(), Modo::TRANSFER_CLINET);
  if (ret) {
    printf("transfer int failed:%s\n", ipstr.c_str());
    return 1;
  }
  transfer.SetReceiveCallback(client_call);
  transfer.StartReceive();
  int i = 0;
  while (1) {
    char data[128] = {0};
    sprintf(data, "client%d", i);
    printf("Send :%s\n", data);
    transfer.Send(Modo::MSG_JPEG, data, strlen(data));
    sleep(1);
    i++;
  }
  return 0;
}
