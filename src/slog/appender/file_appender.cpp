/*
* Created by suemi on 2017/2/16.
*/

#include <sstream>
#include "slog/appender/file_appender.h"
#include "slog/logging/log_guard.h"
#include "slog/utils/file_util.h"
#include "slog/utils/time_util.h"
#include "slog/utils/properties.h"

namespace slog {

using namespace std::chrono;
using time_point = std::chrono::system_clock::time_point;

const long DEFAULT_ROLLING_LOG_SIZE = 10 * 1024 * 1024L;
const long MINIMUM_ROLLING_LOG_SIZE = 200 * 1024L;

FileAppender::FileAppender(const std::string &name, const std::string &filename, std::ios_base::openmode mode,
                           bool create_dirs) : Appender(name), file_path_(filename), mode_(mode),
                                               create_dirs_(create_dirs), is_immediate_(false) {
  Init();
}

FileAppender::FileAppender(const Properties &properties, std::ios_base::openmode mode) : Appender(properties) {
  bool create_dirs;
  bool result = properties.GetBool(create_dirs, "createDirs");
  create_dirs_ = result ? create_dirs : false;

  file_path_ = properties.GetProperty("path", "./log.txt");

  int delay;
  result = properties.GetInt(delay, "reopenDelay");
  reopen_delay_ = result ? delay : 1;
  Init();
}

FileAppender::~FileAppender() {
  if (is_closed_) return;
  Close();
  is_closed_ = true;
}

void FileAppender::Close() {
  out_.close();
  is_closed_ = true;
}

Appender::Result FileAppender::DoAppend(const char *data, int len) {
  if (!out_.good()) {
    if (!Reopen())
      return Result(false, data, len, 0, "file is not open:  " + file_path_);
  }
  auto pos = out_.tellp();
  try {
    out_.write(data, len);
    out_.flush();
  } catch (const std::ios_base::failure &err) {
    return Result(false, data, len, out_.tellp() - pos, err.what());
  }
  return Result(true, data, len, len, std::string());

}

void FileAppender::Open(std::ios_base::openmode mode) {
  if (create_dirs_)
    FileUtil::MakeDirs(file_path_);
  out_.open(file_path_.c_str(), mode);
}

bool FileAppender::Reopen() {
  if (reopen_time_ == time_point() && reopen_delay_ > 0)
    reopen_time_ = TimeUtil::AddSeconds(TimeUtil::Now(), reopen_delay_);
  else if (reopen_time_ < TimeUtil::Now() || reopen_delay_ == 0) {
    out_.close();
    out_.clear();

    Open(std::ios_base::out | std::ios_base::ate | std::ios_base::app);

    reopen_time_ = time_point();
    return out_.good();
  }
  return false;
}

void FileAppender::Init() {
  Open(mode_);
  if (!out_.good())
    LogGuard::Instance()->Error("Unable to open file: " + file_path_);
  out_.exceptions(std::ios_base::failbit | std::ios_base::badbit);
  return;
}

bool FileAppender::is_immediate() const {
  return is_immediate_;
}

void FileAppender::immediate(bool val) {
  is_immediate_ = val;
}

RollingFileAppender::RollingFileAppender(const std::string &name, const std::string &filename, int max_backup,
                                         std::ios_base::openmode mode, bool create_dirs) : FileAppender(name, filename,
                                                                                                        mode,
                                                                                                        create_dirs),
                                                                                           max_backup_(max_backup) {}

RollingFileAppender::RollingFileAppender(const Properties &properties, std::ios_base::openmode mode) : FileAppender(
    properties, mode) {
  int max_backup;
  bool result = properties.GetInt(max_backup, "maxBackup");
  max_backup_ = result ? max_backup : 1;
}

CapacityRollingFileAppender::CapacityRollingFileAppender(const std::string &name, const std::string &filename,
                                                         long max_file_size, int max_backup,
                                                         std::ios_base::openmode mode, bool create_dirs)
    : RollingFileAppender(name, filename, max_backup, mode, create_dirs), capacity_(max_file_size) {
  Init();
}

CapacityRollingFileAppender::CapacityRollingFileAppender(const Properties &properties, std::ios_base::openmode mode)
    : RollingFileAppender(properties, mode) {
  long max_file_size = DEFAULT_ROLLING_LOG_SIZE;
  std::string tmp = properties.GetProperty("maxFileSize");
  if (!tmp.empty()) {
    max_file_size = std::atoi(tmp.c_str());
    if (max_file_size != 0) {
      const auto len = tmp.length();
      if (len > 2 && tmp.compare(len - 2, 2, "MB") == 0)
        max_file_size *= (1024 * 1024); // convert to megabytes
      else if (len > 2 && tmp.compare(len - 2, 2, "KB") == 0)
        max_file_size *= 1024; // convert to kilobytes
    }
  }
  capacity_ = max_file_size;
  Init();
}


Appender::Result CapacityRollingFileAppender::DoAppend(const char *data, int len) {
  if (ShouldRollOver()) RollOver();
  Result res = FileAppender::DoAppend(data, len);
  if (ShouldRollOver()) RollOver();
  return res;

}

void CapacityRollingFileAppender::Init() {
  if (capacity_ < MINIMUM_ROLLING_LOG_SIZE) {
    std::ostringstream oss;
    oss << "RollingFileAppender: MaxFileSize property value is too small. Resetting to" << MINIMUM_ROLLING_LOG_SIZE
        << ".";
    LogGuard::Instance()->Error(oss.str());
    capacity_ = MINIMUM_ROLLING_LOG_SIZE;
  }
  max_backup_ = std::max(max_backup_, 1);
}

void CapacityRollingFileAppender::RollOver() {
  out_.close();
  out_.clear();
  if (max_backup_ > 0) {
    FileUtil::RollOverFiles(file_path_, max_backup_);
    auto target = file_path_ + ".1";
    FileUtil::Rename(file_path_, target);
  }
  Open(std::ios::out | std::ios::trunc);
}

bool CapacityRollingFileAppender::ShouldRollOver() {
  return out_.tellp() > capacity_;
}

CycleRollingFileAppender::CycleRollingFileAppender(const std::string &name, const std::string &filename,
                                                   CycleRollingFileAppender::RollingCycle cycle, int max_backup,
                                                   std::ios_base::openmode mode, bool create_dirs)
    : RollingFileAppender(name, filename, max_backup, mode, create_dirs), cycle_(cycle) {
  Init();
}

CycleRollingFileAppender::CycleRollingFileAppender(const Properties &properties, std::ios_base::openmode mode)
    : RollingFileAppender(properties, mode) {

  int max_backup;
  if (!properties.GetInt(max_backup, "maxBackup")) max_backup_ = 10;
  std::string cycle = properties.GetProperty("cycle", "DAILY");

  if (cycle == "MONTHLY")
    cycle_ = MONTHLY;
  else if (cycle == "WEEKLY")
    cycle_ = WEEKLY;
  else if (cycle == "DAILY")
    cycle_ = DAILY;
  else if (cycle == "TWICE_DAILY")
    cycle_ = TWICE_DAILY;
  else if (cycle == "HOURLY")
    cycle_ = HOURLY;
  else if (cycle == "MINUTELY")
    cycle_ = MINUTELY;
  else {
    LogGuard::Instance()->Error("CycleRollingFileAppender::ctor() - Cycle not valid: " + cycle);
    cycle_ = DAILY;
  }

  Init();
}

void CycleRollingFileAppender::Close() {
  RollOver();
  FileAppender::Close();
}

Appender::Result CycleRollingFileAppender::DoAppend(const char *data, int len) {
  if (TimeUtil::Now() > next_rollover_time_)
    RollOver();
  return FileAppender::DoAppend(data, len);
}

void CycleRollingFileAppender::Init() {
  time_point now = TimeUtil::Now();
  struct tm time = TimeUtil::LocalTime(now);
  time.tm_sec = 0;

  switch (cycle_) {
    case MONTHLY:
      time.tm_mday = 1;
      time.tm_hour = 0;
      time.tm_min = 0;
      break;

    case WEEKLY:
      time.tm_mday -= (time.tm_wday % 7);
      time.tm_hour = 0;
      time.tm_min = 0;
      break;

    case DAILY:
      time.tm_hour = 0;
      time.tm_min = 0;
      break;

    case TWICE_DAILY:
      if (time.tm_hour >= 12) {
        time.tm_hour = 12;
      } else {
        time.tm_hour = 0;
      }
      time.tm_min = 0;
      break;

    case HOURLY:
      time.tm_min = 0;
      break;

    case MINUTELY:
      break;
  };
  now = TimeUtil::FromLocalTime(&time);
  schedule_file_ = GenFileName(now);
  next_rollover_time_ = NextRollOverTime(now);
}

void CycleRollingFileAppender::RollOver() {
  out_.close();
  out_.clear();

  FileUtil::RollOverFiles(schedule_file_, max_backup_);

  std::ostringstream backup_target_oss;
  backup_target_oss << schedule_file_ << "." << 1;
  std::string backup_target = backup_target_oss.str();

  FileUtil::Rename(schedule_file_, backup_target);
  FileUtil::Rename(file_path_, schedule_file_);

  Open(std::ios::out | std::ios::trunc);

  time_point now = TimeUtil::Now();
  if (now > next_rollover_time_) {
    schedule_file_ = GenFileName(now);
    next_rollover_time_ = NextRollOverTime(now);
  }
}

bool CycleRollingFileAppender::ShouldRollOver() {
  return false;
}

FileAppender::Time CycleRollingFileAppender::NextRollOverTime(const FileAppender::Time &t) const {
  switch (cycle_) {
    case MONTHLY: {
      struct tm next_month_time = TimeUtil::LocalTime(t);
      next_month_time.tm_mon += 1;
      next_month_time.tm_isdst = 0;

      time_point ret = TimeUtil::FromLocalTime(&next_month_time);
      if (ret == time_point()) {
        LogGuard::Instance()->Error(
            "DailyRollingFileAppender::calculateNextRolloverTime()- setTime() returned error");
        // Set next rollover to 31 days in future.
        ret = (t + seconds(2678400));
      }

      return ret;
    }

    case WEEKLY:
      return (t + seconds(7 * 24 * 60 * 60));

    case DAILY:
      return (t + seconds(24 * 60 * 60));

    case TWICE_DAILY:
      return (t + seconds(12 * 60 * 60));

    case HOURLY:
      return (t + seconds(60 * 60));

    case MINUTELY:
      return (t + seconds(60));

    default: {
      LogGuard::Instance()->Error("DailyRollingFileAppender::calculateNextRolloverTime()- invalid schedule value");
      return (t + seconds(24 * 60 * 60));  //DAILY
    }
  };
}

std::string CycleRollingFileAppender::GenFileName(const time_point &t) const {
  char const *pattern = 0;
  switch (cycle_) {
    case MONTHLY:
      pattern = "%Y-%m";
      break;
    case WEEKLY:
      pattern = "%Y-%W";
      break;
    case DAILY:
      pattern = "%Y-%m-%d";
      break;
    case TWICE_DAILY:
      pattern = "%Y-%m-%d-%p";
      break;
    case HOURLY:
      pattern = "%Y-%m-%d-%H";
      break;
    case MINUTELY:
      pattern = "%Y-%m-%d-%H-%M";
      break;
    default:
      LogGuard::Instance()->Error("DailyRollingFileAppender::getFilename()- invalid schedule value");
  };

  std::string result(file_path_);
  result += ".";
  result += TimeUtil::Format(t, pattern);
  return result;
}

}

