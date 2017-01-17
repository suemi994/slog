/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_ERROR_HANDLER_H
#define SLOG_ERROR_HANDLER_H

#include "slog/appender/appender.h"

namespace slog {

/**
 * 处理写入失败时的逻辑
 * @author suemi
 */
class ErrorHandler {
public:
  ErrorHandler();

  virtual ~ErrorHandler();

  virtual void Handle(const Appender::Result &result) = 0;

  virtual void Reset() = 0;
};

class IgnoreHandler : public ErrorHandler {
public:
  virtual void Handle(const Appender::Result &result) override;

  virtual void Reset() override;
};

class AbortHandler : public ErrorHandler {
public:

  virtual void Handle(const Appender::Result &result) override;

  virtual void Reset() override;
};

class RetryHandler : public ErrorHandler {
public:

  RetryHandler(int retry_times,std::shared_ptr<Appender> appender);

  virtual void Handle(const Appender::Result &result) override;

  virtual void Reset() override;

private:
  int fail_times_;
  int retry_times_;
  Appender* const owner_;

};

}

#endif 
