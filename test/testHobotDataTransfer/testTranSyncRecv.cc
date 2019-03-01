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
#include <iostream>
#include "HobotDataTransfer/HobotDataTransfer.h"

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
  printf("Listening:%s\n", ipstr.c_str());
  //  Socket to talk to clients
  Modo::HobotDataTransfer transfer;
  int ret = transfer.Init(ipstr.c_str(), Modo::TRANSFER_CLINET);
  if (ret) {
    printf("transfer int failed:%s\n", ipstr.c_str());
    return 1;
  }
  std::cout << "I am here" << std::endl;
  Modo::TransferVector vec;
  transfer.SynchReceive(vec);
  for(auto msg : vec) {
    printf("client_sync_recv[%d,%d,%s] \n", msg.type, msg.datalen, (char *)msg.data);
  } 
  // int i = 0;
  // while (1) {
  //   char data[128] = {0};
  //   sprintf(data, "server%d", i);
  //   printf("Send :%s\n", data);
  //   transfer.Send(1, data, strlen(data));
  //   sleep(1);
  //   i++;
  // }
  transfer.Finish();
  return 0;
}
