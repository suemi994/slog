/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_NULL_APPENDER_H
#define SLOG_NULL_APPENDER_H

#include "slog/appender/appender.h"

namespace slog {

/**
 * 输出到/dev/null，什么也不做的Appender
 * @author suemi
 */
class NullAppender : public Appender {
public:
  NullAppender(const std::string& name="null_appender");

  NullAppender(const Properties &prop);

  virtual bool is_immediate() const override;

protected:

  virtual Result DoAppend(const char *data, int len) override;
};

}

#endif 
