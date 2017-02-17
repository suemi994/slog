/*
* Created by suemi on 2017/2/13.
*/

#include "slog/appender/appender.h"
#include "slog/base/fixed_buffer.h"
#include "slog/appender/console_appender.h"
#include "slog/appender/error_handler.h"
#include "slog/logging/log_guard.h"

namespace slog {

Appender::Result::Result() : Appender::Result(true, nullptr, 0, 0, std::string()) {}

Appender::Result::Result(bool success, const char *start, int length, int write_size, const std::string &msg) : is_success(
    success), data(start), len(length), written(write_size), message(msg) {}

Appender::Appender(const std::string &name) : name_(name),is_closed_(false) {}

Appender::~Appender() {
  if(!is_closed_)
    LogGuard::Instance()->Error("Derived Appender did not release resources.");
}

std::shared_ptr<Appender> Appender::DefaultInstance() {
}

void Appender::Close() {
  is_closed_ = true;
}

bool Appender::IsClosed() const {
  return is_closed_;
}

template<int SIZE>
void Appender::Append(const FixedBuffer<SIZE> &buffer) {
  Append(buffer.data(), buffer.length());
}

void Appender::Append(const char *data, int len) {
  auto res = DoAppend(data,len);
  if(!res.is_success && error_handler_!= nullptr)
    error_handler_->Handle(res);
}

const std::string &Appender::name() const {
  return name_;
}

std::unique_ptr<ErrorHandler> &Appender::error_handler() const {
  return error_handler_;
}

void Appender::set_error_handler(const std::unique_ptr<ErrorHandler> &handler) {
  error_handler_ = std::move(handler);
}

}