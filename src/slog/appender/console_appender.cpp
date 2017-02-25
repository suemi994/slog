/*
* Created by suemi on 2017/2/13.
*/

#include <iostream>
#include "slog/appender/console_appender.h"

namespace slog {

ConsoleAppender::ConsoleAppender(const std::string &name) : Appender(name) {}

Appender::Result ConsoleAppender::DoAppend(const char *data, int len) {
  std::cout.write(data,len);
  std::cout.flush();
  return Result(true,data,len,len,std::string());
}

}