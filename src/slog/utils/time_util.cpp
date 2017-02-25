/*
* Created by suemi on 2017/2/14.
*/

#include <sstream>
#include <iomanip>
#include <vector>

#include "slog/utils/time_util.h"

namespace slog {

using namespace std::chrono;
using time_point = std::chrono::system_clock::time_point;

time_point TimeUtil::Now() {
  return system_clock::now();
}

long TimeUtil::Seconds(time_point timestamp) {
  return time_point_cast<seconds>(timestamp).time_since_epoch().count();
}

long TimeUtil::MilliSeconds(time_point timestamp) {
  return time_point_cast<milliseconds>(timestamp).time_since_epoch().count();
}

long TimeUtil::MicroSeconds(time_point timestamp) {
  return time_point_cast<microseconds>(timestamp).time_since_epoch().count();
}

std::string TimeUtil::Format(time_point timestamp, const std::string &fmt) {
  std::stringstream out;
  std::time_t tmp = system_clock::to_time_t(timestamp);
  out << std::put_time(std::localtime(&tmp), fmt.c_str());
  return out.str();

/*
  using namespace std;

  string formated_array;
  string formated;

  if (fmt.empty() || fmt[0] == 0)
    return string();

  tm time = LocalTime(timestamp);
  bool isText = true;
  // Walk the format string and process all occurences of %q, %Q and %s.

  for (string::const_iterator it = fmt.begin(); it != fmt.end(); ++it) {
    if (isText) {
      if (*it == '%')
        isText = false;
      else
        formated_array.push_back(*it);
    } else {
      formated_array.push_back('%');
      formated_array.push_back(*it);
      isText = true;
    }
  }

  // Finally call strftime/wcsftime to format the rest of the string.

  formated.swap(formated_array);
  std::size_t bufSize = formated.size() + 1;
  std::size_t len;

  // Limit how far can the buffer grow. This is necessary so that we
  // catch bad format string. Some implementations of strftime() signal
  // both too small buffer and invalid format string by returning 0
  // without changing errno.
  std::size_t const maxBufferSize = (std::max)(static_cast<std::size_t>(1024), bufSize * 16);

  vector<char> szbuf;
  do {
    szbuf.resize(bufSize);
    errno = 0;
    len = std::strftime(&szbuf[0], bufSize, formated.c_str(), &time);
    if (len == 0) {
      int const eno = errno;
      bufSize *= 2;
      if (bufSize > maxBufferSize) {
        LogGuard::Instance()->Error("Error in strftime(): " + StringUtil::ConvertInt(eno));
      }
    }
  } while (len == 0);

  return string(szbuf.begin(), szbuf.begin() + len);
*/
}

time_point TimeUtil::AddSeconds(time_point timestamp, long secs) {
  return timestamp + seconds(secs);
}

time_point TimeUtil::AddMilliSeconds(time_point timestamp, long msecs) {
  return timestamp + milliseconds(msecs);
}

time_point TimeUtil::AddMicroSeconds(time_point timestamp, long usecs) {
  return timestamp + microseconds();
}

struct tm TimeUtil::LocalTime(time_point timestamp) {
  std::time_t tmp = system_clock::to_time_t(timestamp);
  struct tm res;
  ::localtime_r(&tmp, &res);
  return res;
}

time_point TimeUtil::FromLocalTime(struct tm *src) {
  std::time_t tmp = std::mktime(src);
  return tmp == -1 ? time_point() : system_clock::from_time_t(tmp);
}

}
