//
// Created by liuhuo.chu on 2016/11/10.
//


#include <chrono>
#include <ctime>
#include <string>
#include <thread>

#ifdef _WIN32

#else
#include <sys/time.h>
#endif
#include <string.h>
#include <time.h>
#include <zmq.h>
#include "TimeSync/TimeSync.h"
#define MODULE_TAG "TimeSync"
namespace Modo {

int64_t TimeSync::start_epoch_time_ = 0L;
int64_t TimeSync::init_time_stamp_;
int TimeSync::multiple_ = 1;
int TimeSync::divider_ = 1;

int64_t GetSysStamp() {
  std::chrono::milliseconds time =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());

  int64_t stamp = time.count();
  // LOGE_T(MODULE_TAG) << "HobotADAS::GetTimeStamp():" << stamp;
  return stamp;
}
void TimeSync::Reset() {
  start_epoch_time_ = GetSysStamp();
  init_time_stamp_ = start_epoch_time_;
  multiple_ = 1;
  divider_ = 1;
}

void TimeSync::SetTimeZone(const char *tz) {
  // putenv(const_cast<char *>(tz));
}

void TimeSync::InitTimeUtil(TimeStamp curEpochTime, int multiple, int divider) {
  start_epoch_time_ = GetSysStamp();
  init_time_stamp_ = curEpochTime;
  multiple_ = multiple;
  divider_ = divider;
}

TimeStamp TimeSync::GetEpochTimeMs() {
  return init_time_stamp_ +
         (GetSysStamp() - start_epoch_time_) * multiple_ / divider_;
}
TimeStamp TimeSync::GetEpochTimeMs(TimeStamp src_stamp) {
  return init_time_stamp_ +
         (src_stamp - start_epoch_time_) * multiple_ / divider_;
}
void TimeSync::GetTimeRatio(int *pMultiple, int *pDivider) {
  *pMultiple = multiple_;
  *pDivider = divider_;
}

void TimeSync::SleepUtil(TimeStamp epochTimeMs) {
  TimeStamp time = start_epoch_time_ +
                   (epochTimeMs - init_time_stamp_) * divider_ / multiple_;
  std::this_thread::sleep_until(
      std::chrono::system_clock::time_point(std::chrono::milliseconds(time)));
}

void TimeSync::SleepFor(TimeStamp timeMs) {
  std::this_thread::sleep_for(
      std::chrono::milliseconds(timeMs * multiple_ / divider_));
}

void TimeSync::TimeMstoStringMs(TimeStamp timeMs, std::string &result) {
  auto ttime_t = std::chrono::system_clock::to_time_t(
      std::chrono::system_clock::time_point(std::chrono::milliseconds(timeMs)));
  auto tp_sec = std::chrono::system_clock::from_time_t(ttime_t);
  int ms = (timeMs % 1000);

  std::tm *ttm = localtime(&ttime_t);
  char date_time_format[] = "%Y%m%d-%H%M%S";
  char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";
  strftime(time_str, strlen(time_str), date_time_format, ttm);
  sprintf(&time_str[strlen(time_str)], "_%03d", ms);
  result = time_str;
}

void TimeSync::TimeMstoStringMs_Sys(TimeStamp timeMs, std::string &result) {
  int ms = (timeMs % 1000);
  time_t tt = timeMs / 1000;

  std::tm *ttm = localtime(&tt);
  char date_time_format[] = "%Y%m%d-%H%M%S";
  char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";
  strftime(time_str, strlen(time_str), date_time_format, ttm);
  sprintf(&time_str[strlen(time_str)], "_%03d", ms);
  result = time_str;
}

//#ifdef SUPPORT_TIME_SYNC
std::thread TimeSync::timesync_service_thread_;
bool TimeSync::is_running_;
void *TimeSync::service_sender_;
void *TimeSync::service_receiver_;
void *TimeSync::service_ctx_;
int64_t TimeSync::device_time_offset_;

void TimeSync::StartTimeSyncService() {
  //  SetLogLevel(HOBOT_LOG_DEBUG);
  is_running_ = true;
  if (!timesync_service_thread_.joinable()) {
    timesync_service_thread_ = std::thread(&TimeSync::TimeSyncThreadFun);
  }
}

void TimeSync::StopTimeSyncService() {
  zmq_close(service_receiver_);
  zmq_close(service_sender_);
  zmq_ctx_destroy(service_ctx_);
  is_running_ = false;
  timesync_service_thread_.join();
}

enum {
  MSG_REQUEST_TIMESTAMP,
  MSG_RESPOSE_TIMESTAMP,
  MSG_NOTIFY_NEW_FILE,
  MSG_NOTIFY_UPDATE_TIMESTAMP,
  MSG_NOTIFY_DEVICE_ERROR
};

typedef struct {
  uint32_t msg_type;
  int64_t time;
} time_sync_msg_t;

bool TimeSync::ClientSyncTime(std::string ip_addr, int max_repeat) {
  //  SetLogLevel(HOBOT_LOG_DEBUG);
  std::string server;
  device_time_offset_ = 0;
  {
    if (ip_addr.substr(0, 3) != "tcp") {
      server = "tcp://";
      server += ip_addr;
    } else {
      server = ip_addr;
    }
    int pos = server.find_last_of(':');
    if (pos == 3) {
      server += ":" TIME_SYNC_SERVICE_PORT;
    }
  }

  std::string endpoint = server;
  int pos = endpoint.find_last_of(':') + 1;
  int zmq_to = 500;

  void *ctx = zmq_ctx_new();
  endpoint.replace(endpoint.begin() + pos, endpoint.end(),
                   TIME_SYNC_RESPONSE_PORT);
  void *recv_sock = zmq_socket(ctx, ZMQ_PULL);
  zmq_setsockopt(recv_sock, ZMQ_RCVTIMEO, &zmq_to, sizeof(int));
  int rc = zmq_connect(recv_sock, endpoint.c_str());
  if (rc) {
    // LOGE_T(MODULE_TAG) << "[time_sync] connect TIME_SYNC_RESPONSE_PORT
    // failed";
    zmq_close(recv_sock);
    zmq_ctx_destroy(ctx);
    return false;
  }

  endpoint.replace(endpoint.begin() + pos, endpoint.end(),
                   TIME_SYNC_SERVICE_PORT);
  void *send_sock = zmq_socket(ctx, ZMQ_PUSH);
  zmq_setsockopt(send_sock, ZMQ_SNDTIMEO, &zmq_to, sizeof(int));
  int zmq_linger = 100;
  zmq_setsockopt(send_sock, ZMQ_LINGER, &zmq_linger, sizeof(int));
  rc = zmq_connect(send_sock, endpoint.c_str());
  if (rc) {
    // LOGE_T(MODULE_TAG) << "[time_sync] connect TIME_SYNC_SERVICE_PORT
    // failed";
    zmq_close(send_sock);
    zmq_close(recv_sock);
    zmq_ctx_destroy(ctx);
    return false;
  }
  int64_t t_start, t_end, pc_time, sync_dev_time = 0, sync_pc_time = 0;
  int64_t sync_duration, min_sync_duration = 2000;
  for (int i_sync = 0; i_sync < max_repeat && min_sync_duration > 6; i_sync++) {
    time_sync_msg_t msg;
    time_sync_msg_t msg_rec;
    msg.msg_type = MSG_REQUEST_TIMESTAMP;
    msg.time = 0;
    // LOGD_T(MODULE_TAG) << "[time_sync] Sending time sync request at " <<
    // i_sync;
    t_start = TimeSync::GetEpochTimeMs();
    int len = sizeof(time_sync_msg_t);
    int retry = 10;
    zmq_send(send_sock, (void *)&msg, len, 0);
    do {
      rc = zmq_recv(recv_sock, (void *)&msg_rec, sizeof(time_sync_msg_t), 0);
      retry--;
    } while (msg_rec.msg_type != MSG_RESPOSE_TIMESTAMP && retry > 0);

    if (retry <= 0)
      break;
    t_end = TimeSync::GetEpochTimeMs();
    pc_time = (t_start + t_end) / 2;
    //    LOGD_T(MODULE_TAG) << "[time_sync] Msg type = " << msg_rec.msg_type
    //                       << ", time=" << msg_rec.time;
    sync_duration = t_end - t_start;
    if (sync_duration < min_sync_duration) {
      min_sync_duration = sync_duration;
      sync_dev_time = msg_rec.time;
      sync_pc_time = pc_time;
      //      LOGD_T(MODULE_TAG) << "[time_sync] Timesync duration= " <<
      //      sync_duration
      //                         << ", dev time: " << sync_dev_time
      //                         << ", pctime: " << sync_pc_time;
    }
    //    LOGD_T(MODULE_TAG) << "[time_sync] Timesync once, st: " << t_start
    //                       << ", end: " << t_end << ", error: " <<
    //                       sync_duration / 2
    //                       << ", diff: " << (t_end + t_start) / 2 -
    //                       msg_rec.time;
  }

  if (sync_dev_time) {
    TimeSync::InitTimeUtil(
        sync_dev_time - sync_pc_time + TimeSync::GetEpochTimeMs(), 1, 1);
    device_time_offset_ = sync_pc_time - sync_dev_time;
  } else {
    //    LOGE_T(MODULE_TAG) << "[time_sync] Timesync failed!!";
  }

  zmq_close(recv_sock);
  zmq_close(send_sock);
  zmq_ctx_destroy(ctx);

  return sync_dev_time;
}

void TimeSync::TimeSyncThreadFun() {
  //  LOGD_T(MODULE_TAG) << "TimeSyncThreadFun +";

  service_ctx_ = zmq_ctx_new();

  service_receiver_ = zmq_socket(service_ctx_, ZMQ_PULL);
  int rc = zmq_bind(service_receiver_, "tcp://*:" TIME_SYNC_SERVICE_PORT);
  // assert(rc == 0);
  service_sender_ = zmq_socket(service_ctx_, ZMQ_PUSH);
  rc = zmq_bind(service_sender_, "tcp://*:" TIME_SYNC_RESPONSE_PORT);
  // assert(rc == 0);
  // int timeout = 250;
  // rc = zmq_setsockopt(mZmqReceiver, ZMQ_RCVTIMEO, &timeout, sizeof(int));

  time_sync_msg_t msg_rec;
  while (rc == 0 && is_running_) {
    int ret = zmq_recv(service_receiver_, (void *)&msg_rec,
                       sizeof(time_sync_msg_t), 0);
    if (ret != -1) {
      if (msg_rec.msg_type == MSG_REQUEST_TIMESTAMP) {
        time_sync_msg_t msg_send;
        msg_send.msg_type = MSG_RESPOSE_TIMESTAMP;
        msg_send.time = GetEpochTimeMs();
        zmq_send(service_sender_, (void *)&msg_send, sizeof(time_sync_msg_t),
                 0);
        //        LOGD_T(MODULE_TAG) << "Receive time sync msg, response time: "
        //                           << msg_send.time;
      };
    } else {
      //      LOGD_T(MODULE_TAG) << "zmq_rec error = " << errno;
    }
  }
  //  LOGD_T(MODULE_TAG) << "TimeSyncThreadFun -";
}

int64_t TimeSync::GetDeviceTimeOffset() { return device_time_offset_; }
//#endif

int64_t TimeSync::GetSysTimeMs() {
  //	SYSTEMTIME sys;
  //	int64_t result;
  //	GetLocalTime(&sys);
  //	struct tm temptm;
  //	temptm.tm_year = sys.wYear - 1900;
  //	temptm.tm_mon = sys.wMonth - 1;
  //	temptm.tm_mday = sys.wDay;
  //	temptm.tm_hour = sys.wHour;
  //	temptm.tm_min = sys.wMinute;
  //	temptm.tm_sec = sys.wSecond;
  //	temptm.tm_isdst = 0;
  //	temptm.tm_yday = 0;
  //	result = mktime(&temptm);
  //	return result*1000 + sys.wMilliseconds;
  return 0;
}


TimeStamp GetTimeStamp() { return TimeSync::GetEpochTimeMs(); }

TimeRegister::TimeRegister(const std::string info, int verbose,
                           const char *file, int line) {
  info_ = info;
  verbose_ = verbose;
  toc_ = GetTimeStamp();
  this->line = line;
  this->file = file;
}

TimeRegister::~TimeRegister() {
  toc_ = GetTimeStamp() - toc_;
  //#if (GOOGLE_STRIP_LOG == 0)
  //  char buf[1024];
  //  snprintf(buf, sizeof(buf), "#%06llddms: %s", toc_, info_.c_str());
  //  LOGE_T(MODULE_TAG) << buf;
  //  if (VLOG_IS_ON(verbose_)) {
  //#ifdef __ANDROID__
  //    ALOGI("%s\t%d\t%s", file, line, buf);
  //#endif
  //    google::LogMessage(file, line, google::GLOG_INFO).stream() << buf;
  //  }
  //#endif
}
}
