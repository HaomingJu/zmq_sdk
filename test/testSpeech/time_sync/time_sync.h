//
// Created by liuhuo.chu on 2017/04/1
//

#ifndef ADAS_time_utils_H
#define ADAS_time_utils_H


#include <stdint.h>
#include <ctime>
#include <string>
//#ifndef USE_SDK_PROTO
#define SUPPORT_TIME_SYNC
//#endif
#ifdef SUPPORT_TIME_SYNC
#include <zmq.h>
#include <thread>
#define TIME_SYNC_SERVICE_PORT "55555"
#define TIME_SYNC_RESPONSE_PORT "55556"
#endif
#define PACK_RECV_PORT "55560"


namespace Modo {

typedef int64_t TimeStamp;

TimeStamp GetTimeStamp();

class TimeUtils {
 public:
  static TimeStamp GetEpochTimeMs();
  static void SleepUtil(TimeStamp epochTimeMs);
  static void SleepFor(TimeStamp timeMs);
  static void SetTimeZone(const char *tz);
  static void Reset();
  static void InitTimeUtil(TimeStamp curEpochTime, int multiple, int divider);
  static void GetTimeRatio(int *pMultiple, int *pDivider);
  static void TimeMstoStringMs(TimeStamp timeMs, std::string &result);
  static void TimeMstoStringMs_Sys(TimeStamp timeMs, std::string &result);
  static int64_t GetSysTimeMs();

 private:
  static TimeStamp start_epoch_time_;
  static TimeStamp init_time_stamp_;
  static int multiple_;
  static int divider_;

  //#ifdef SUPPORT_TIME_SYNC
 public:
  static void StartTimeSyncService();
  static void StopTimeSyncService();
  static bool ClientSyncTime(std::string ip_addr, int max_repeat);
  static void TimeSyncThreadFun();
  static int64_t GetDeviceTimeOffset();

 private:
  static std::thread timesync_service_thread_;
  static bool is_running_;
  static void *service_ctx_;
  static void *service_sender_;
  static void *service_receiver_;
  static int64_t device_time_offset_;
  //#endif
};

class TimeRegister {
 protected:
  TimeStamp toc_;
  std::string info_;
  const char *file;
  int line;
  int verbose_;

 public:
  TimeRegister(const std::string info, int verbose, const char *file, int line);
  ~TimeRegister();
};

#define TIME_REGISTER_VERBOSE(info, verbose) \
  TimeRegister timer(info, verbose, __FILE__, __LINE__)
#define TIME_REGISTER(info) TIME_REGISTER_VERBOSE(info, LOG_VERBOSE_LOW)
}

#endif  // ADAS_time_utils_H
