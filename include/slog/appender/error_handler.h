/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_ERROR_HANDLER_H
#define SLOG_ERROR_HANDLER_H

#include "slog/appender/appender.h"

namespace slog {

class ErrorHandler {
public:
  ErrorHandler();
  virtual ~ErrorHandler();

  virtual void Handler(const Appender::Result& result) = 0;
  virtual void Reset() = 0;
};

class IgnoreHandler : public ErrorHandler {

};

class RetryHandler : public ErrorHandler {

};

}

#endif 
