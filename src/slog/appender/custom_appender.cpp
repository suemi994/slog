/*
* Created by suemi on 2017/2/13.
*/

#include "slog/appender/custom_appender.h"

namespace slog {

CustomAppender::CustomAppender(const std::string &name) : Appender(name) {}

CustomAppender::~CustomAppender() {
  if(!IsClosed()){
    Close();
    is_closed_ = true;
  }
}

CustomAppender::CustomFunc &CustomAppender::func() const {
  return func_;
}

void CustomAppender::set_func(const CustomAppender::CustomFunc &func) {
  func_ = func;
}

Appender::Result CustomAppender::DoAppend(const char *data, int len) {
  return func_(data,len);
}

bool CustomAppender::is_immediate() const {
  return is_immediate_;
}

void CustomAppender::immediate(bool val) {
  is_immediate_ = val;
}

}