/*
* Created by suemi on 2017/2/13.
*/

#ifndef SLOG_CUSTOM_APPENDER_H
#define SLOG_CUSTOM_APPENDER_H

#include <functional>
#include "appender.h"

namespace slog {

class CustomAppender : public Appender {
public:
  using CustomFunc = std::function<Result(const char*,int)>;

  CustomAppender(const std::string& name);

  virtual ~CustomAppender() override;

  CustomFunc& func() const;

  void set_func(const CustomFunc& func);

protected:

  virtual Result DoAppend(const char* data,int len) override;

  CustomFunc func_;
};

}

#endif 
