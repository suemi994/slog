/*
* Created by suemi on 2017/1/18.
*/

#include "slog/filter/filter_builder.h"

namespace slog {

FilterPtr FilterBuilder::accept_all_ = std::make_shared<Filter>(new AcceptAllFilter());
FilterPtr FilterBuilder::deny_all_ = std::make_shared<Filter>(new DenyAllFilter());

FilterBuilder &FilterBuilder::DenyAll() {
  filter_ = deny_all_;
  return *this;
}

FilterBuilder &FilterBuilder::AcceptAll() {
  filter_ = accept_all_;
  return *this;
}

FilterBuilder &FilterBuilder::LevelMatch(LogLevel level) {
  filter_ = std::make_shared<LogLevelMatchFilter>(level);
  return *this;
}

FilterBuilder &FilterBuilder::LevelRange(LogLevel min, LogLevel max) {
  filter_ = std::make_shared<LogLevelRangeFilter>(min,max);
  return *this;
}

FilterBuilder &FilterBuilder::And(const FilterPtr filter) {
  filter_ = std::make_shared<BinaryAndFilter>(filter_== nullptr?accept_all_:filter_,filter);
  return *this;
}

FilterBuilder &FilterBuilder::And(const FilterList &filters) {
  FilterList tmp = filters;
  if(filter_) tmp.push_back(filter_);
  filter_ = std::make_shared<AndFilter>(tmp);
  return *this;
}

FilterBuilder &FilterBuilder::Or(const FilterPtr filter) {
  filter_ = std::make_shared<BinaryOrFilter>(filter_== nullptr?deny_all_:filter_,filter);
  return *this;
}

FilterBuilder &FilterBuilder::Or(const FilterList &filters) {
  FilterList tmp = filters;
  if(filter_) tmp.push_back(filter_);
  filter_ = std::make_shared<OrFilter>(tmp);
  return *this;
}

FilterBuilder &FilterBuilder::Not(const FilterPtr filter) {
  filter_ = std::make_shared<NotFilter>(filter);
  return *this;
}

FilterBuilder &FilterBuilder::Not() {
  filter_ = std::make_shared<NotFilter>(filter_);
  return *this;
}

FilterPtr FilterBuilder::Build() const {
  return filter_?filter_:accept_all_;
}

FilterBuilder &FilterBuilder::Reset() {
  filter_.reset();
  return *this;
}

}