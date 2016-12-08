/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FILTER_H
#define SLOG_FILTER_H

#include <memory>

#include "slog/logging/log_level.h"

namespace slog {

class LogEvent;

class Properties;

class Filter;

using FilterPtr = std::shared_ptr<Filter>;

class Filter {
public:
  enum class Result {
    DENY,
    ACCEPT
  };

  Filter() = default;

  virtual ~Filter();

  virtual Result Decide(LogEvent &log_event) const = 0;

  virtual bool Equals(const Filter &filter) const;

};

class CompositeFilter : public Filter {

};

class AndFilter : public CompositeFilter {

};

class OrFilter : public CompositeFilter {

};

class BinaryFilter : public Filter {

};

class BinaryAndFilter : public BinaryFilter {

};

class BinaryOrFilter : public BinaryFilter {

};

class DenyAllFilter : public Filter {
public:
  DenyAllFilter();

  DenyAllFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;
};

inline bool operator==(const DenyAllFilter &left, const DenyAllFilter &right) {
  return true;
}

class AcceptAllFilter : public Filter {
public:
  AcceptAllFilter();

  AcceptAllFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;
};

inline bool operator==(const AcceptAllFilter &left, const AcceptAllFilter &right) {
  return true;
}

class LogLevelMatchFilter : public Filter {
public:
  LogLevelMatchFilter();

  LogLevelMatchFilter(const Properties &properties);

  LogLevelMatchFilter(LogLevel level);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  LogLevel match_level_;

  friend bool operator==(const LogLevelMatchFilter &, const LogLevelMatchFilter &);
};

inline bool operator==(const LogLevelMatchFilter &left, const LogLevelMatchFilter &right) {
  return left.match_level_ == right.match_level_;
}


class LogLevelRangeFilter : public Filter {
public:
  LogLevelRangeFilter();

  LogLevelRangeFilter(LogLevel min, LogLevel max);

  LogLevelRangeFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  LogLevel min_level_;
  LogLevel max_level_;

  friend bool operator==(const LogLevelRangeFilter &, const LogLevelRangeFilter &);
};

inline bool operator==(const LogLevelRangeFilter &left, const LogLevelRangeFilter &right) {
  return left.min_level_ == right.min_level_ && left.max_level_ == right.max_level_;
}

}
#endif
