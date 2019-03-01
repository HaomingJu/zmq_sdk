/*
 * testTranClient.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
//#include <zmq.h>
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "LinuxHisfSDK/hobotSpeechAPI.h"


#define TIME_LONG 8
#define AUDIO_LEN 4

int64_t count = 0;
char *audio_;

Modo::HobotDataTransfer g_transfer;
int g_i = 0;
void send(AudioInfo *audio_data) {
  //  unsigned long long timestamp;
  //  std::vector<char> audio_v;
  //  size_t audio_size = sizeof(audio_data->audio_channel);
  //  audio_v.resize(audio_size + 8);
  // // timestamp = htonll(audio_data->timestamp);
  //  memcpy(audio_v.data(), &timestamp, 8);
  //  memcpy(audio_v.data() + 8, audio_data->audio_channel, 1536);
  //  // audio_ = (char*)malloc(1544);
  //  // memcpy(audio_, &timestamp, 8);
  //  // memcpy(audio_ + 8, audio_data->audio_channel, 1536);
  //  servrer.SendData(audio_v.data(), 1544, nullptr);
  //  std::cout << "time stamp = " << audio_data->timestamp << std::endl;
  //  std::cout << "audio size : " << audio_size << std::endl;
  //  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  printf("transfer Send stamp=%lld\n", audio_data->timestamp);
  Modo::TransferVector tans_vector;
  char buff[2048];
  {
    Modo::DataTransferInputMsg msg;
    msg.type = Modo::MSG_AUDIO_TIME_STAMP;
    msg.data = (int8_t *)&audio_data->timestamp;
    msg.datalen = 8;
    tans_vector.push_back(msg);
  }

  {
    Modo::DataTransferInputMsg msg1;
    msg1.type = Modo::MSG_AUDIO;
    sprintf(buff, "hello_%d", g_i++);
    msg1.data = (int8_t *)audio_data->audio_channel;
    ;
    msg1.datalen = 6 * DATA_FRAME_16MS;
    tans_vector.push_back(msg1);
  }

  int ret = g_transfer.Send(tans_vector);
  if (ret) {
    printf("transferSend failed\n");
  } else {
    printf("transferSend sucess\n");
  }

  // free(audio_);
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
  int ret;

  ret = g_transfer.Init(ipstr.c_str(), Modo::TRANSFER_CLINET);
  if (ret) {
    printf("transfer int failed:%s\n", ipstr.c_str());
    return 1;
  }

  AudioInfo info;
  while (1) {
    send(&info);
    // sync_time();
    usleep(1600);
  }
  g_transfer.Finish();
  hobot::speech::Finish();
  hobot::speech::Finalize();
  return 0;
}
