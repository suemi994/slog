/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_APPENDER_H
#define SLOG_APPENDER_H

#include <string>
#include <memory>
#include "slog/utils/no_copyable.h"
#include "slog/base/fixed_buffer.h"

namespace slog {

class ErrorHandler;

class Appender : public NoCopyable {
public:

  struct Result {
    bool is_success;
    std::string message;
    char* data;
    int written;
    int len;
  };

  Appender();
  virtual ~Appender();

  static std::shared_ptr<Appender> DefaultInstance();

  /**
   * 关闭所持有的资源比如文件句柄、网络socket
   */
  virtual void Close() = 0;

  bool IsClosed() const ;

  template<int SIZE>
  virtual void Append(const FixedBuffer<SIZE>& buffer) = 0;

  virtual void Append(const char* data, int len) = 0;

  const std::string& name() const;

  std::unique_ptr<ErrorHandler>& error_handler() const ;

  void set_error_handler(std::unique_ptr<ErrorHandler>&& handler);

protected:

  virtual Result DoAppend(const char* data, int len);

  std::string name_;
  std::unique_ptr<ErrorHandler> error_handler_;

  friend class ErrorHandler;
};

}

#endif 
