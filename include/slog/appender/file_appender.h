/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FILE_APPENDER_H
#define SLOG_FILE_APPENDER_H

#include <ios>
#include <chrono>
#include <fstream>
#include "appender.h"

namespace slog {

class Properties;

class FileAppender : public Appender {
  using Time = std::chrono::system_clock::time_point;
public:
  FileAppender(const std::string &name,
               const std::string &filename,
               std::ios_base::openmode mode = std::ios_base::trunc,
               bool create_dirs = false);

  FileAppender(const Properties &properties, std::ios_base::openmode mode = std::ios_base::trunc);

  virtual ~FileAppender() override;

  virtual void Close() override;

  virtual bool is_immediate() const override;

  void immediate(bool val);

protected:

  virtual Result DoAppend(const char *data, int len) override;

  void Open(std::ios_base::openmode mode);

  bool Reopen();

  virtual void Init();

  std::string file_path_;
  int reopen_delay_;
  bool create_dirs_;
  std::ios_base::openmode mode_;
  Time reopen_time_;
  std::ofstream out_;
  bool is_immediate_;
};

class RollingFileAppender : public FileAppender {
public:
  RollingFileAppender(const std::string &name,
                      const std::string &filename,
                      int max_backup = 1,
                      std::ios_base::openmode mode = std::ios_base::trunc,
                      bool create_dirs = false);

  RollingFileAppender(const Properties &properties, std::ios_base::openmode mode = std::ios_base::trunc);
protected:
  virtual void RollOver() = 0;

  virtual bool ShouldRollOver() = 0;

  int max_backup_;
};

class CapacityRollingFileAppender : public RollingFileAppender {
public:
  CapacityRollingFileAppender(const std::string &name,
                              const std::string &filename,
                              long max_file_size = 10 * 1024 * 1024,
                              int max_backup = 1,
                              std::ios_base::openmode mode = std::ios_base::trunc,
                              bool create_dirs = false);

  CapacityRollingFileAppender(const Properties &properties, std::ios_base::openmode mode = std::ios_base::trunc);
protected:

  virtual Result DoAppend(const char *data, int len) override;

  virtual void Init() override;

  virtual void RollOver() override;

  virtual bool ShouldRollOver() override;

  long capacity_;
};

class CycleRollingFileAppender : public RollingFileAppender {
public:
  enum RollingCycle {
    MONTHLY, WEEKLY, DAILY, TWICE_DAILY, HOURLY, MINUTELY
  };

  CycleRollingFileAppender(const std::string &name,
                           const std::string &filename,
                           RollingCycle cycle = DAILY,
                           int max_backup = 1,
                           std::ios_base::openmode mode = std::ios_base::trunc,
                           bool create_dirs = false);

  CycleRollingFileAppender(const Properties &properties, std::ios_base::openmode mode = std::ios_base::trunc);

  virtual void Close() override;

protected:

  virtual Result DoAppend(const char *data, int len) override;

  virtual void Init() override;

  virtual void RollOver() override;

  virtual bool ShouldRollOver() override;

  Time NextRollOverTime(const Time& t) const;

  std::string GenFileName(const Time& t) const;

  RollingCycle cycle_;

  std::string schedule_file_;

  Time next_rollover_time_;

};

}

#endif 
