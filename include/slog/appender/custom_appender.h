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

  CustomAppender(const Properties &prop);

  virtual ~CustomAppender() override;

  const CustomFunc &func() const;

  void set_func(const CustomFunc& func);

  virtual bool is_immediate() const override;

  void immediate(bool val);

protected:

  virtual Result DoAppend(const char* data,int len) override;

  CustomFunc func_;

  bool is_immediate_;
};

}

#endif 
