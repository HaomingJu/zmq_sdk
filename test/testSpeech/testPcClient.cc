/*
 * testTranClient.cc
 *
 *  Created on: Feb 10, 2019
 *      Author: user
 */


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "HobotDataTransfer/HobotDataTransfer.h"
#include "base/base.h"
//#include <zmq.h>
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "LinuxHisfSDK/hobotSpeechAPI.h"
#include "TimeSync/TimeSync.h"
//主机序转网络序
unsigned long long htonll(unsigned long long val) {
  if (__BYTE_ORDER == __LITTLE_ENDIAN) {
    return (((unsigned long long)htonl((int)((val << 32) >> 32))) << 32) |
           (unsigned int)htonl((int)(val >> 32));
  } else if (__BYTE_ORDER == __BIG_ENDIAN) {
    return val;
  }
}

#define TIME_LONG 8
#define AUDIO_LEN 4

int64_t count = 0;
char *audio_;

Modo::HobotDataTransfer g_transfer;
int g_i = 0;
void audio_cb(AudioInfo *audio_data) {
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
    msg.type = 2002;
    msg.data = (int8_t *)&audio_data->timestamp;
    msg.datalen = 8;
    tans_vector.push_back(msg);
  }

  {
    Modo::DataTransferInputMsg msg1;
    msg1.type = 2001;
    sprintf(buff, "hello_%d", g_i++);
    msg1.data = (int8_t *)audio_data->audio_channel;
    msg1.datalen = 6 * DATA_FRAME_16MS;
    tans_vector.push_back(msg1);
  }

  while (true) {
    int ret = g_transfer.Send(tans_vector, 1000);
    if (ret) {
      printf("transfer Send failed\n");
    }
    break;
  }


  // free(audio_);
}
int sync_time() {
  Modo::TimeSync::ClientSyncTime("192.168.1.10", 5);
  int64_t stamp = Modo::TimeSync::GetEpochTimeMs();
  std::cout << "GetEpochTimeMs, stamp = " << stamp << std::endl;
  hobot::speech::SetTimeStamp(stamp);
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
  int ret;

  ret = g_transfer.Init(ipstr.c_str(), Modo::TRANSFER_CLINET);
  if (ret) {
    printf("transfer int failed:%s\n", ipstr.c_str());
    return 1;
  }
  SpeechConfig config;
  config.audio_cb = audio_cb;
  config.save_file_enable = false;
  ret = hobot::speech::Init(config);
  if (ret != 0) {
    std::cout << "Audio Init Failed, ret = " << ret << std::endl;
    return 0;
  }
  sync_time();
  ret = hobot::speech::Start();
  if (ret != 0) {
    std::cout << "Start Failed, ret = " << ret << std::endl;
  }
  std::cout << "do, stamp = " << std::endl;
  while (1) {
    sync_time();
    sleep(10);
  }
  g_transfer.Finish();
  hobot::speech::Finish();
  hobot::speech::Finalize();
  return 0;
}
