/*
* Created by suemi on 2017/1/17.
*/
#include <stdexcept>
#include "slog/appender/error_handler.h"

namespace slog {

ErrorHandler::ErrorHandler() {}

ErrorHandler::~ErrorHandler() {}

void IgnoreHandler::Handle(const Appender::Result &result) {}

void IgnoreHandler::Reset() {}

void AbortHandler::Handle(const Appender::Result &result) {
  throw std::runtime_error("Can't append buffer!");
}

void AbortHandler::Reset() {}

RetryHandler::RetryHandler(int retry_times, std::shared_ptr<Appender> appender) : retry_times_(retry_times),
                                                                                  fail_times_(0), owner_(appender.get()) {

}

void RetryHandler::Handle(const Appender::Result &result) {
  if(result.is_success) return;
  if(++fail_times_<=retry_times_ && owner_)
    owner_->Append(result.data+result.written,result.len-result.written);
}

void RetryHandler::Reset() {
  fail_times_ = 0;
}

}
