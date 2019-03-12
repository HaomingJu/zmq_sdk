/*
 * testClient.cc
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */


#include <stdio.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "base/base.h"
int main(int argc, char **argv) {
  Modo::HobotDataTransfer transfer;
  transfer.Init("", Modo::TRANSFER_SEVER);
  Modo::TransferVector msgs;
  // todo:完善sample code
  while (1) {
    transfer.Send(msgs);
    sleep(1);
  }
  sleep(10);
  return 0;
}
