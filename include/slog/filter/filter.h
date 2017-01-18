/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FILTER_H
#define SLOG_FILTER_H

#include <memory>
#include <vector>

#include "slog/logging/log_level.h"

namespace slog {

class LogEvent;

class Properties;

class Filter;

using FilterPtr = std::shared_ptr<Filter>;
using FilterList = std::vector<FilterPtr>;

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
public:
  CompositeFilter() = default;

  CompositeFilter(const FilterList &filters);

  void Append(FilterPtr filter);

  void Remove(FilterPtr filter);

  void Clear();

protected:
  FilterList filters_;

};

inline bool operator==(const FilterList &lhs, const FilterList &rhs) {
  auto len = lhs.size();
  if (len != rhs.size()) return false;
  for (auto i = 0; i < len; ++i)
    if (!lhs[i]->Equals(*rhs[i]))
      return false;
  return true;
}

inline bool operator!=(const FilterList &lhs, const FilterList &rhs) {
  return !(lhs == rhs);
}

class AndFilter : public CompositeFilter {
public:
  AndFilter() = default;

  AndFilter(const FilterList &filters);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  friend bool operator==(const AndFilter &lhs, const AndFilter &rhs);

  friend bool operator!=(const AndFilter &lhs, const AndFilter &rhs);
};

inline bool operator==(const AndFilter &lhs, const AndFilter &rhs) {
  return slog::operator==(lhs.filters_, rhs.filters_);
}

inline bool operator!=(const AndFilter &lhs, const AndFilter &rhs) {
  return !(lhs == rhs);
}

class OrFilter : public CompositeFilter {
public:
  OrFilter() = default;

  OrFilter(const FilterList &filters);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  friend bool operator==(const OrFilter &lhs, const OrFilter &rhs);

  friend bool operator!=(const OrFilter &lhs, const OrFilter &rhs);
};


inline bool operator==(const OrFilter &lhs, const OrFilter &rhs) {
  return slog::operator==(lhs.filters_, rhs.filters_);
}

inline bool operator!=(const OrFilter &lhs, const OrFilter &rhs) {
  return !(lhs == rhs);
}

class BinaryFilter : public Filter {
public:
  BinaryFilter(FilterPtr left, FilterPtr right);

protected:
  FilterPtr left_;
  FilterPtr right_;
};

class BinaryAndFilter : public BinaryFilter {
public:
  BinaryAndFilter(FilterPtr left, FilterPtr right);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:

  friend bool operator==(const BinaryAndFilter &lhs, const BinaryAndFilter &rhs);

  friend bool operator!=(const BinaryAndFilter &lhs, const BinaryAndFilter &rhs);
};

inline bool operator==(const BinaryAndFilter &lhs, const BinaryAndFilter &rhs) {
  return (lhs.left_->Equals(*rhs.left_) && lhs.right_->Equals(*rhs.right_)) ||
         (lhs.left_->Equals(*rhs.right_) && lhs.right_->Equals(*rhs.left_));
}

inline bool operator!=(const BinaryAndFilter &lhs, const BinaryAndFilter &rhs) {
  return !(lhs == rhs);
}

class BinaryOrFilter : public BinaryFilter {
public:
  BinaryOrFilter(FilterPtr left, FilterPtr right);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:

  friend bool operator==(const BinaryOrFilter &lhs, const BinaryOrFilter &rhs);

  friend bool operator!=(const BinaryOrFilter &lhs, const BinaryOrFilter &rhs);
};

inline bool operator==(const BinaryOrFilter &lhs, const BinaryOrFilter &rhs) {
  return (lhs.left_->Equals(*rhs.left_) && lhs.right_->Equals(*rhs.right_)) ||
         (lhs.left_->Equals(*rhs.right_) && lhs.right_->Equals(*rhs.left_));
}

inline bool operator!=(const BinaryOrFilter &lhs, const BinaryOrFilter &rhs) {
  return !(lhs == rhs);
}

class NotFilter : public Filter {
public:
  NotFilter(FilterPtr base);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  FilterPtr base_;

  friend bool operator==(const NotFilter &lhs, const NotFilter &rhs);

  friend bool operator!=(const NotFilter &lhs, const NotFilter &rhs);
};

inline bool operator==(const NotFilter &lhs, const NotFilter &rhs) {
  return lhs.Equals(*rhs.base_);
}

inline bool operator!=(const NotFilter &lhs, const NotFilter &rhs) {
  return !(lhs == rhs);
}

class DenyAllFilter : public Filter {
public:
  DenyAllFilter() = default;

  DenyAllFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;
};

inline bool operator==(const DenyAllFilter &left, const DenyAllFilter &right) {
  return true;
}

inline bool operator!=(const DenyAllFilter &left, const DenyAllFilter &right) {
  return false;
}

class AcceptAllFilter : public Filter {
public:
  AcceptAllFilter() = default;

  AcceptAllFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;
};

inline bool operator==(const AcceptAllFilter &left, const AcceptAllFilter &right) {
  return true;
}

inline bool operator!=(const AcceptAllFilter &left, const AcceptAllFilter &right) {
  return false;
}

class LogLevelMatchFilter : public Filter {
public:

  LogLevelMatchFilter(const Properties &properties);

  LogLevelMatchFilter(LogLevel level = LogLevel::DEBUG);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  LogLevel match_level_;

  friend bool operator==(const LogLevelMatchFilter &, const LogLevelMatchFilter &);

  friend bool operator!=(const LogLevelMatchFilter &, const LogLevelMatchFilter &);
};

inline bool operator==(const LogLevelMatchFilter &left, const LogLevelMatchFilter &right) {
  return left.match_level_ == right.match_level_;
}

inline bool operator!=(const LogLevelMatchFilter &left, const LogLevelMatchFilter &right) {
  return !(left == right);
}


class LogLevelRangeFilter : public Filter {
public:

  LogLevelRangeFilter(LogLevel min = LogLevel::TRACE, LogLevel max = LogLevel::ERROR);

  LogLevelRangeFilter(const Properties &properties);

  virtual Result Decide(LogEvent &log_event) const override;

  virtual bool Equals(const Filter &filter) const override;

private:
  LogLevel min_level_;
  LogLevel max_level_;

  friend bool operator==(const LogLevelRangeFilter &, const LogLevelRangeFilter &);

  friend bool operator!=(const LogLevelRangeFilter &, const LogLevelRangeFilter &);
};

inline bool operator==(const LogLevelRangeFilter &left, const LogLevelRangeFilter &right) {
  return left.min_level_ == right.min_level_ && left.max_level_ == right.max_level_;
}

inline bool operator!=(const LogLevelRangeFilter &left, const LogLevelRangeFilter &right) {
  return !(left == right);
}

}
#endif
