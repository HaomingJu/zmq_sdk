/*
 * testTranSever.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <stdio.h>
#include <string.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "base/base.h"
FILE *g_fp;
int server_call(Modo::TransferVector &tran_vector) {
  printf("server_call \n");
  int size = tran_vector.size();
  for (int i = 0; i < size; i++) {
    Modo::DataTransferInputMsg msg = tran_vector[i];
    printf("server_call[%d,%d,%s] \n", msg.type, msg.datalen, msg.data);
    if (msg.type == 6) {
      fwrite(msg.data, msg.datalen, 1, g_fp);
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
  g_fp = fopen("./recoder.pcm", "wb+");
  if (!g_fp) {
    printf("recoder open failed\n");
    return 1;
  }
  while (1) {
    SLEEP(1);
  }
  return 0;
}
