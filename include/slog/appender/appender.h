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
  using Buffer = FixedBuffer<detail::LARGE_BUFFER_SIZE>;
public:

  struct Result {
    bool is_success;
    std::string message;
  };

  Appender();
  virtual ~Appender();

  virtual void Close() = 0;

  bool IsClosed() const ;

  virtual Result Append(const Buffer& buffer) = 0;

  const std::string& name() const;

  std::shared_ptr<ErrorHandler> error_handler() const ;

  void set_error_handler(std::shared_ptr<ErrorHandler> handler);

protected:
  std::string name_;
  std::shared_ptr<ErrorHandler> error_handler_;
};

}

#endif 
