/*
* Created by suemi on 2016/12/13.
*/

#include <cassert>

#include "slog/logging/log_scheduler.h"
#include "slog/appender/appender.h"
#include "slog/utils/time_util.h"
#include "slog/logging/log_guard.h"
#include "slog/utils/properties.h"

namespace slog {

LogScheduler::LogScheduler(int flush_interval) : flush_interval_(flush_interval), latch_(1), thread_(nullptr),
                                                 is_running_(
                                                     false) {
  current_buffer_ = std::make_unique<Buffer>();
  next_buffer_ = std::make_unique<Buffer>();
  current_buffer_->Bezero();
  next_buffer_->Bezero();
  buffers_.reserve(16);
}

LogScheduler::LogScheduler(const Properties &properties) : LogScheduler(3){
  int interval;
  if(properties.GetInt(interval,"flushInterval"))
    flush_interval_ = interval;
}

LogScheduler::~LogScheduler() {
  if (is_running_)
    Stop();
  if (thread_) delete thread_;
}

void LogScheduler::Append(const char *buf, size_t len) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto &appender:immediate_appenders_) appender->Append(buf, len);
  if (current_buffer_->avail() > len) {
    current_buffer_->Append(buf, len);
    return;
  }
  buffers_.push_back(current_buffer_.release());

  // 当前缓冲区已满，使用第一备用缓冲区
  if (next_buffer_)
    current_buffer_ = std::move(next_buffer_);
  else
    current_buffer_ = std::make_unique<Buffer>();
  current_buffer_->Append(buf, len);
  cond_.notify_one();
}

void LogScheduler::Flush(BufferPtr &buf) {
  for (auto &appender:delay_appenders_)
    appender->Append(*buf);
}

void LogScheduler::Start() {
  is_running_ = true;
  thread_ = new std::thread(std::bind(&LogScheduler::DaemonFunc, this));
  latch_.Wait(); // 确定写入线程开启后离开调用栈
}

void LogScheduler::Stop() {
  is_running_ = false;
  cond_.notify_all();
  thread_->join();
}

void LogScheduler::AddAppender(std::shared_ptr<Appender> appender_ptr) {
  auto &dst = appender_ptr->is_immediate() ? immediate_appenders_ : delay_appenders_;
  dst.push_back(appender_ptr);
}

void LogScheduler::RemoveAllAppenders() {
  immediate_appenders_.clear();
  delay_appenders_.clear();
}

void LogScheduler::RemoveAppender(std::shared_ptr<Appender> appender_ptr) {
  for (auto it = immediate_appenders_.begin(); it != immediate_appenders_.end(); ++it)
    if (*it == appender_ptr) {
      immediate_appenders_.erase(it);
      break;
    }

  for (auto it = delay_appenders_.begin(); it != delay_appenders_.end(); ++it)
    if (*it == appender_ptr) {
      delay_appenders_.erase(it);
      break;
    }
}

void LogScheduler::RemoveAppender(const std::string &appender_name) {
  for (auto it = immediate_appenders_.begin(); it != immediate_appenders_.end(); ++it)
    if ((*it)->name() == appender_name) {
      immediate_appenders_.erase(it);
      break;
    }

  for (auto it = delay_appenders_.begin(); it != delay_appenders_.end(); ++it)
    if ((*it)->name() == appender_name) {
      delay_appenders_.erase(it);
      break;
    }
}

bool LogScheduler::is_running() const {
  return is_running_;
}

void LogScheduler::DaemonFunc() {
  assert(is_running_);
  latch_.CountDown();
  BufferPtr backup_1 = std::make_unique<Buffer>();
  BufferPtr backup_2 = std::make_unique<Buffer>();
  backup_1->Bezero();
  backup_2->Bezero();

  BufferList to_write;
  to_write.reserve(16);

  while (is_running_) {
    assert(backup_1 && backup_1->length() == 0);
    assert(backup_2 && backup_2->length() == 0);
    assert(to_write.empty());

    {
      // 交换已满缓存区和待写缓存区，并更新两个主写缓冲(current_buffer和next_buffer)
      std::unique_lock<std::mutex> lock(mutex_);
      if (buffers_.empty())
        cond_.wait_for(lock, std::chrono::seconds(flush_interval_), [this]() {
          return !buffers_.empty();
        });
      buffers_.push_back(current_buffer_.release());
      current_buffer_ = std::move(backup_1);
      to_write.swap(buffers_);
      if (!next_buffer_)
        next_buffer_ = std::move(backup_2);
    }

    assert(!to_write.empty());

    // 待写缓存区过多，警告后丢弃
    if (to_write.size() > 25) {
      char buf[256];
      snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
               TimeUtil::Format(TimeUtil::Now(), "%F %T").c_str(),
               to_write.size() - 2);
      LogGuard::Instance()->Error(buf);

      to_write.erase(to_write.begin() + 2, to_write.end());
    }

    // 实际刷写日志内容
    for (auto &buf:to_write)
      Flush(buf);

    // 更新主要待交换的备用缓冲区

    if (to_write.size() > 2)
      to_write.resize(2);

    if (!backup_1) {
      assert(!to_write.empty());
      backup_1 = to_write.back();
      to_write.pop_back();
      backup_1->Reset();
    }

    if (!backup_2) {
      assert(!to_write.empty());
      backup_2 = to_write.back();
      to_write.pop_back();
      backup_2->Reset();
    }

    to_write.clear();
  }

}

}

