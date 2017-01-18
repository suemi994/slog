/*
* Created by suemi on 2017/1/18.
*/

#include <stdexcept>
#include "slog/filter/filter.h"
#include "slog/logging/log_event.h"

namespace slog {

Filter::~Filter() {}

bool Filter::Equals(const Filter &filter) const {
  return this == &filter;
}

CompositeFilter::CompositeFilter(const FilterList &filters) : filters_(filters) {}

void CompositeFilter::Append(FilterPtr filter) {
  filters_.push_back(filter);
}

void CompositeFilter::Remove(FilterPtr filter) {
  //此处删除严格按照对象的内存地址相同来
  for(auto it=filters_.begin();it!=filters_.end();++it)
    if(*it==filter){
      filters_.erase(it);
      break;
    }
}

void CompositeFilter::Clear() {
  filters_.clear();
}

AndFilter::AndFilter(const FilterList &filters) : CompositeFilter(filters) {}

Filter::Result AndFilter::Decide(LogEvent &log_event) const {
  for(auto & f:filters_)
    if(f->Decide(log_event)==Result::DENY)
      return Result::DENY;
  return Result::ACCEPT;
}

bool AndFilter::Equals(const Filter &filter) const {
  const AndFilter* ptr = dynamic_cast<AndFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}


OrFilter::OrFilter(const FilterList &filters) : CompositeFilter(filters) {}

Filter::Result OrFilter::Decide(LogEvent &log_event) const {
  for(auto & f:filters_)
    if(f->Decide(log_event)==Result::ACCEPT)
      return Result::ACCEPT;
  return Result::DENY;
}

bool OrFilter::Equals(const Filter &filter) const {
  const OrFilter* ptr = dynamic_cast<OrFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

BinaryFilter::BinaryFilter(FilterPtr left, FilterPtr right) : left_(left),right_(right) {}

BinaryAndFilter::BinaryAndFilter(FilterPtr left, FilterPtr right) : BinaryFilter(left, right) {}

Filter::Result BinaryAndFilter::Decide(LogEvent &log_event) const {
  return left_->Decide(log_event)==Result::ACCEPT && right_->Decide(log_event)==Result::ACCEPT?Result::ACCEPT:Result::DENY;
}

bool BinaryAndFilter::Equals(const Filter &filter) const {
  const BinaryAndFilter* ptr = dynamic_cast<BinaryAndFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

BinaryOrFilter::BinaryOrFilter(FilterPtr left, FilterPtr right) : BinaryFilter(left, right) {}

Filter::Result BinaryOrFilter::Decide(LogEvent &log_event) const {
  return left_->Decide(log_event)==Result::ACCEPT || right_->Decide(log_event)==Result::ACCEPT?Result::ACCEPT:Result::DENY;
}

bool BinaryOrFilter::Equals(const Filter &filter) const {
  const BinaryOrFilter* ptr = dynamic_cast<BinaryOrFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

NotFilter::NotFilter(FilterPtr base) :base_(base) {}

Filter::Result NotFilter::Decide(LogEvent &log_event) const {
  return base_->Decide(log_event)==Result::ACCEPT?Result::DENY:Result::ACCEPT;
}

bool NotFilter::Equals(const Filter &filter) const {
  const NotFilter* ptr = dynamic_cast<NotFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

Filter::Result DenyAllFilter::Decide(LogEvent &log_event) const {
  return Result::DENY;
}

bool DenyAllFilter::Equals(const Filter &filter) const {
  const DenyAllFilter* ptr = dynamic_cast<DenyAllFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

Filter::Result AcceptAllFilter::Decide(LogEvent &log_event) const {
  return Result::ACCEPT;
}

bool AcceptAllFilter::Equals(const Filter &filter) const {
  const AcceptAllFilter* ptr = dynamic_cast<AcceptAllFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

LogLevelMatchFilter::LogLevelMatchFilter(LogLevel level) : match_level_(level) {}

Filter::Result LogLevelMatchFilter::Decide(LogEvent &log_event) const {
  return log_event.log_level()>=match_level_?Result::ACCEPT:Result::DENY;
}

bool LogLevelMatchFilter::Equals(const Filter &filter) const {
  const LogLevelMatchFilter* ptr = dynamic_cast<LogLevelMatchFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

LogLevelRangeFilter::LogLevelRangeFilter(LogLevel min, LogLevel max) :min_level_(min),max_level_(max){
  if(min>max)
    throw std::invalid_argument("Max level should be greater than min level in LevelRangeFilter!");
}

Filter::Result LogLevelRangeFilter::Decide(LogEvent &log_event) const {
  return log_event.log_level()>=min_level_ && log_event.log_level()<=max_level_?Result::ACCEPT:Result::DENY;
}

bool LogLevelRangeFilter::Equals(const Filter &filter) const {
  const LogLevelRangeFilter* ptr = dynamic_cast<LogLevelRangeFilter*>(&filter);
  if(!ptr) return false;
  return slog::operator==(*this,*ptr);
}

}