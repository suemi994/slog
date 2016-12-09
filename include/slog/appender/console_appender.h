/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_CONSOLE_APPENDER_H
#define SLOG_CONSOLE_APPENDER_H

#include "appender.h"

namespace slog {

class Appender;
class Appender::Result;

class ErrorHandler {
public:
  ErrorHandler();
  virtual ~ErrorHandler();

  virtual void Handle(const Appender::Result& err) = 0;
  virtual void Reset();
};

}

#endif 
