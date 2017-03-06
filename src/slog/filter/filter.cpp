/*
* Created by suemi on 2017/1/18.
*/

#include <cassert>
#include "slog/filter/filter.h"
#include "slog/logging/log_event.h"
#include "slog/utils/properties.h"
#include "slog/filter/filter_factory.h"

namespace slog {

Filter::~Filter() {}

bool Filter::Equals(const Filter &filter) const {
  return this == &filter;
}

CompositeFilter::CompositeFilter(const FilterList &filters) : filters_(filters) {
  for (auto &ptr:filters) assert(ptr != nullptr);
}

CompositeFilter::CompositeFilter(const Properties &prop) {
  auto children = prop.PropertyWithoutSuffix();
  for (auto &child:children) {
    auto ptr = FilterFactory::CreateInstance(child.second, prop.GetPropertySubset(child.first + "."));
    assert(ptr);
    filters_.push_back(std::move(ptr));
  }
}

void CompositeFilter::Append(FilterPtr filter) {
  filters_.push_back(filter);
}

void CompositeFilter::Remove(FilterPtr filter) {
  //此处删除严格按照对象的内存地址相同来
  for (auto it = filters_.begin(); it != filters_.end(); ++it)
    if (*it == filter) {
      filters_.erase(it);
      break;
    }
}

void CompositeFilter::Clear() {
  filters_.clear();
}

AndFilter::AndFilter(const FilterList &filters) : CompositeFilter(filters) {}

AndFilter::AndFilter(const Properties &prop) : CompositeFilter(prop) {}

Filter::Result AndFilter::Decide(const LogEvent &log_event) const {
  for (auto &f:filters_)
    if (f->Decide(log_event) == Result::DENY)
      return Result::DENY;
  return Result::ACCEPT;
}

bool AndFilter::Equals(const Filter &filter) const {
  const AndFilter *ptr = dynamic_cast<const AndFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}


OrFilter::OrFilter(const FilterList &filters) : CompositeFilter(filters) {}

OrFilter::OrFilter(const Properties &prop) : CompositeFilter(prop) {}

Filter::Result OrFilter::Decide(const LogEvent &log_event) const {
  for (auto &f:filters_)
    if (f->Decide(log_event) == Result::ACCEPT)
      return Result::ACCEPT;
  return Result::DENY;
}

bool OrFilter::Equals(const Filter &filter) const {
  const OrFilter *ptr = dynamic_cast<const OrFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

BinaryFilter::BinaryFilter(FilterPtr left, FilterPtr right) : left_(left), right_(right) {
  assert(left != nullptr && right != nullptr);
}

BinaryFilter::BinaryFilter(const Properties &prop) {
  auto children = prop.PropertyWithoutSuffix();
  assert(children.size() == 2);
  auto it = children.begin();
  left_ = FilterFactory::CreateInstance(it->second, prop.GetPropertySubset(it->first + "."));
  assert(left_);
  ++it;
  right_ = FilterFactory::CreateInstance(it->second, prop.GetPropertySubset(it->first + "."));
}

BinaryAndFilter::BinaryAndFilter(FilterPtr left, FilterPtr right) : BinaryFilter(left, right) {}

BinaryAndFilter::BinaryAndFilter(const Properties &prop) : BinaryFilter(prop) {}

Filter::Result BinaryAndFilter::Decide(const LogEvent &log_event) const {
  return left_->Decide(log_event) == Result::ACCEPT && right_->Decide(log_event) == Result::ACCEPT ? Result::ACCEPT
                                                                                                   : Result::DENY;
}

bool BinaryAndFilter::Equals(const Filter &filter) const {
  const BinaryAndFilter *ptr = dynamic_cast<const BinaryAndFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

BinaryOrFilter::BinaryOrFilter(FilterPtr left, FilterPtr right) : BinaryFilter(left, right) {}

BinaryOrFilter::BinaryOrFilter(const Properties &prop) : BinaryFilter(prop) {}

Filter::Result BinaryOrFilter::Decide(const LogEvent &log_event) const {
  return left_->Decide(log_event) == Result::ACCEPT || right_->Decide(log_event) == Result::ACCEPT ? Result::ACCEPT
                                                                                                   : Result::DENY;
}

bool BinaryOrFilter::Equals(const Filter &filter) const {
  const BinaryOrFilter *ptr = dynamic_cast<const BinaryOrFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

NotFilter::NotFilter(FilterPtr base) : base_(base) {
  assert(base != nullptr);
}

NotFilter::NotFilter(const Properties &prop) {
  auto children = prop.PropertyWithoutSuffix();
  assert(children.size() == 1);
  auto it = children.begin();
  base_ = FilterFactory::CreateInstance(it->second, prop.GetPropertySubset(it->first + "."));
}

Filter::Result NotFilter::Decide(const LogEvent &log_event) const {
  return base_->Decide(log_event) == Result::ACCEPT ? Result::DENY : Result::ACCEPT;
}

bool NotFilter::Equals(const Filter &filter) const {
  const NotFilter *ptr = dynamic_cast<const NotFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

DenyAllFilter::DenyAllFilter(const Properties &properties) {}

Filter::Result DenyAllFilter::Decide(const LogEvent &log_event) const {
  return Result::DENY;
}

bool DenyAllFilter::Equals(const Filter &filter) const {
  const DenyAllFilter *ptr = dynamic_cast<const DenyAllFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

AcceptAllFilter::AcceptAllFilter(const Properties &properties) {}

Filter::Result AcceptAllFilter::Decide(const LogEvent &log_event) const {
  return Result::ACCEPT;
}

bool AcceptAllFilter::Equals(const Filter &filter) const {
  const AcceptAllFilter *ptr = dynamic_cast<const AcceptAllFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

LogLevelMatchFilter::LogLevelMatchFilter(const Properties &properties) {
  auto str = properties.GetProperty("LogLevelToMatch");
  try {
    match_level_ = slog::FromString(str);
  } catch (...) {
    match_level_ = LogLevel::INFO;
  }
}

LogLevelMatchFilter::LogLevelMatchFilter(LogLevel level) : match_level_(level) {}

Filter::Result LogLevelMatchFilter::Decide(const LogEvent &log_event) const {
  return log_event.log_level() >= match_level_ ? Result::ACCEPT : Result::DENY;
}

bool LogLevelMatchFilter::Equals(const Filter &filter) const {
  const LogLevelMatchFilter *ptr = dynamic_cast<const LogLevelMatchFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

LogLevelRangeFilter::LogLevelRangeFilter(LogLevel min, LogLevel max) : min_level_(min), max_level_(max) {
  assert(max >= min);
}

LogLevelRangeFilter::LogLevelRangeFilter(const Properties &properties) {
  auto min_str = properties.GetProperty("LogLevelMin");
  auto max_str = properties.GetProperty("LogLevelMax");

  try {
    min_level_ = slog::FromString(min_str);
  } catch (...) {
    min_level_ = LogLevel::TRACE;
  }

  try {
    max_level_ = slog::FromString(max_str);
  } catch (...) {
    max_level_ = LogLevel::ERROR;
  }
}

Filter::Result LogLevelRangeFilter::Decide(const LogEvent &log_event) const {
  return log_event.log_level() >= min_level_ && log_event.log_level() <= max_level_ ? Result::ACCEPT : Result::DENY;
}

bool LogLevelRangeFilter::Equals(const Filter &filter) const {
  const LogLevelRangeFilter *ptr = dynamic_cast<const LogLevelRangeFilter *>(&filter);
  if (!ptr) return false;
  return slog::operator==(*this, *ptr);
}

}