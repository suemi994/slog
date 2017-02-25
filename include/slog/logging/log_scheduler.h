/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOG_SCHEDULER_H
#define SLOG_LOG_SCHEDULER_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "slog/utils/no_copyable.h"
#include "slog/base/fixed_buffer.h"

namespace slog {

class Appender;

/**
 * 集中处理所有的日志写入工作，委托各个Appender实际输出到各个目的输出
 * @author suemi
 */
class LogScheduler : public NoCopyable {
  using AppenderList = std::vector<std::shared_ptr<Appender>>;
  using Buffer = FixedBuffer<detail::LARGE_BUFFER_SIZE>;
  using BufferPtr = std::unique_ptr<Buffer>;
  using BufferList = std::vector<BufferPtr>;
public:
  LogScheduler(int flush_interval);
  ~LogScheduler();

  static std::shared_ptr<LogScheduler> DefaultInstance();

  void Append(const char* buf, int len);

  void Flush();

  void Start();

  void Stop();

  void AddAppender(std::shared_ptr<Appender> appender_ptr);

  void RemoveAllAppenders();

  void RemoveAppender(std::shared_ptr<Appender> appender_ptr);

  void RemoveAppender(const std::string& appender_name);

  bool is_running() const;

private:

  void DaemonFunc();

  AppenderList delay_appenders_;
  AppenderList immediate_appenders_;

  bool is_running_;
  const int flush_interval_;
  std::thread thread_;

  std::condition_variable cond_;
  std::mutex mutex_;

  BufferPtr current_buffer_;
  BufferPtr next_buffer_;
  BufferList buffers_;
  BufferList buffers_to_write_;
};

}

#endif 
