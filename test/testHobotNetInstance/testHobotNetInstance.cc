/*
 * testHobotNetInstance.cc
 *
 *  Created on: Dec 25, 2018
 *      Author: user
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>
#include <string>
#include "HobotNetwork/HobotNetworkInstance.h"
#include "HobotNetwork/HobotZmqServer.h"
#include "HobotNetwork/HobotZmqClient.h"

int main(int argc, char **argv) {
  // std::string ip;
  // std::string port;
  // std::string ipstr;

  // /* you can use the following code to test client instance */

  // // if (argc < 3) {
  // //   ipstr = "tcp://localhost:5555";
  // // } else {
  // //   ip = argv[1];
  // //   port = argv[2];
  // //   ipstr = "tcp://" + ip + ":" + port;
  // // }
  // // HobotNetworkInstance m_ins;
  // // HobotClientBase *client = m_ins.CreateClientInstance(HOBOT_ZMQ);
  // // client->Init(ipstr.c_str());
  // // int bufflen = 2 * 1024 * 1024;
  // // void *buf = malloc(bufflen);
  // // memset(buf, 0, bufflen);
  // // for (int i = 0 ; i < 10 ; ++i) {
  // //   client->SendData("Hello", 6);
  // //   size_t buflen = 10;
  // //   void *buf = malloc(buflen);
  // //   int sz = client->RecvData(buf, buflen);
  // //   printf("recv datasize = %d \n", sz);
  // // }
  // // client->Finish();
  // // m_ins.DestroyClientInstance(client);

  // /* you can use the following code to test server instance */
  // if (argc < 3) {
  //   ipstr = "tcp://*:5555";
  // } else {
  //   ip = argv[1];
  //   port = argv[2];
  //   ipstr = "tcp://" + ip + ":" + port;
  // }
  // HobotNetworkInstance m_ins;
  // HobotServerBase *server = m_ins.CreateServerInstance(HOBOT_ZMQ);
  // server->Init(ipstr.c_str());
  // char str[] = "World";
  // while (true) {
  //   size_t buflen = 1024 * 1024;
  //   void *buf = malloc(buflen);
  //   memset(buf, 0, buflen);
  //   int sz = server->RecvData(buf, buflen);
  //   printf("recv datasize = %d", sz);
  //   // printf("begin Send \n");
  //   // printf("begin Send=%s \n", str);
  //   server->SendData("World", 6);
  //   printf("Send=%s \n", str);
  //   sleep(1);
  // }
  // server->Finish();
  // m_ins.DestroyServerInstance(server);
  return 0;
}