/*****************************************************************************
 * Copyright (c) 2017 Horizon Robotics. All rights reserved.
 * Created by chen.tang@horizon.ai
 * Data : 2018/12/28
 *****************************************************************************/

#ifndef _HOBOT_DEFINE_H_
#define _HOBOT_DEFINE_H_

#define MAX_MIC_NUM 3

// 16毫秒数据长度
#define DATA_FRAME_16MS 256  // 16毫秒数据长度

//处理后的音频数据类型
#define AUDIO_TYPE_DEFAULT 0  //默认唤醒数据
#define AUDIO_TYPE_ASR 1      // asr 识别数据

struct AudioInfo {
  // int audio_type;                             //hisf处理数据的类型，唤醒数据/
  // ASR识别数据
  short audio_channel[3 * DATA_FRAME_16MS];  //通道1数据
  unsigned long long timestamp;
  // short audio_channel1[2 * DATA_FRAME_16MS];  //通道2数据
  // short doa_data[MAX_MIC_NUM * DATA_FRAME_16MS]; //doa信息
  // int vad_flag1;                              //通道1 vad信息
  // int vad_flag2;                              //通道2 vad信息
};

/****************************************************************************
 * info  : 语音获取注册回调函数
 * param : audio_data：hisf处理后的音频数据
 ***************************************************************************/
typedef void (*hobot_audio_data_callback)(struct AudioInfo *audio_data);


#endif
