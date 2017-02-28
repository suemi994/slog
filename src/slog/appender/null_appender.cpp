/*
* Created by suemi on 2017/2/13.
*/

#include "slog/appender/null_appender.h"

namespace slog {

NullAppender::NullAppender(const std::string &name) : Appender(name) {}

NullAppender::NullAppender(const Properties &prop) : Appender(prop) {}

Appender::Result NullAppender::DoAppend(const char *data, int len) {
  return Result(true, data, len, len, std::string());
}

bool NullAppender::is_immediate() const {
  return false;
}

}
