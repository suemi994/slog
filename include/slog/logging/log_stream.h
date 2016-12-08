/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOG_STREAM_H
#define SLOG_LOG_STREAM_H

#include "slog/utils/no_copyable.h"

namespace slog {

class Logger;

/**
 * 用户直接使用的接口，日志输入的前端
 * @author suemi
 */
class LogStream : public NoCopyable {
public:
  LogStream() = default;

  LogStream &operator<<(bool val);
  LogStream &operator<<(char val);
  LogStream &operator<<(short val);
  LogStream &operator<<(unsigned short val);
};

}

#endif 
