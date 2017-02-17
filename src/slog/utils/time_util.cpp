/*
* Created by suemi on 2017/2/14.
*/

#include <sstream>
#include <iomanip>
#include "slog/utils/time_util.h"

namespace slog {

std::chrono::time_point TimeUtil::Now() {
  return steady_clock::now();
}

long TimeUtil::Seconds(std::chrono::time_point timestamp) {
  return time_point_cast<seconds>(timestamp).time_since_epoch().count();
}

long TimeUtil::MilliSeconds(std::chrono::time_point timestamp){
  return time_point_cast<milliseconds>(timestamp).time_since_epoch().count();
}

long TimeUtil::MicroSeconds(std::chrono::time_point timestamp) {
  return time_point_cast<microseconds>(timestamp).time_since_epoch().count();
}

std::string TimeUtil::Format(std::chrono::time_point timestamp, const std::string &fmt) {
  std::stringstream out;
  std::time_t tmp = system_clock::to_time_t(timestamp);
  out<< std::put_time(std::localtime(&tmp),fmt.c_str());
  return out.str();
}

std::chrono::time_point TimeUtil::AddSeconds(std::chrono::time_point timestamp, long secs) {
  return timestamp + seconds(secs);
}

std::chrono::time_point TimeUtil::AddMilliSeconds(std::chrono::time_point timestamp, long msecs) {
  return timestamp + milliseconds(msecs);
}
std::chrono::time_point TimeUtil::AddMicroSeconds(std::chrono::time_point timestamp, long usecs) {
  return timestamp + microseconds();
}

struct tm TimeUtil::LocalTime(std::chrono::time_point timestamp) {
  std::time_t tmp = system_clock::to_time_t(timestamp);
  struct tm res;
  ::localtime_r(&tmp,&res);
  return res;
}

std::chrono::time_point TimeUtil::FromLocalTime(struct tm *src) {
  std::time_t tmp = std::mktime(src);
  return tmp==-1?time_point():system_clock::from_time_t(tmp);
}

}
