/*
* Created by suemi on 2016/12/12.
*/

#ifndef SLOG_TIME_HELPER_H
#define SLOG_TIME_HELPER_H

#include <chrono>
#include <string>
#include <ctime>

namespace slog {

/*
 * 管理时间戳相关的帮助类
 * @author suemi
 */
class TimeUtil {
  using namespace std::chrono;
  using time_point = std::chrono::system_clock::time_point;
public:
  static time_point Now();

  static long Seconds(time_point timestamp);

  static long MilliSeconds(time_point timestamp);

  static long MicroSeconds(time_point timestamp);

  static std::string Format(time_point timestamp,const std::string& fmt);

  static time_point AddSeconds(time_point timestamp, long seconds);

  static time_point AddMilliSeconds(time_point timestamp, long msecs);

  static time_point AddMicroSeconds(time_point timestamp, long usecs);

  static struct tm LocalTime(time_point timestamp);

  static time_point FromLocalTime(struct tm* src);
};

}

#endif 
