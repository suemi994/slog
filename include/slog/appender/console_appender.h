/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_CONSOLE_APPENDER_H
#define SLOG_CONSOLE_APPENDER_H

#include "appender.h"

namespace slog {

class Properties;

class ConsoleAppender : public Appender {
public:
  ConsoleAppender(const std::string &name);

  ConsoleAppender(const Properties& prop);

  virtual bool is_immediate() const override;

protected:

  virtual Result DoAppend(const char *data, int len) override;

};

}

#endif 
