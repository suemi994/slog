/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FILTER_BUILDER_H
#define SLOG_FILTER_BUILDER_H

#include "slog/filter/filter.h"

namespace slog {

/**
 * 建造者模式，方便快速组装Filter
 * @author suemi
 */
class FilterBuilder {
public:
  FilterBuilder &DenyAll();

  FilterBuilder &AcceptAll();

  FilterBuilder &LevelMatch(LogLevel level);

  FilterBuilder &LevelRange(LogLevel min, LogLevel max);

  FilterBuilder &Connect(const FilterPtr filter);

  FilterPtr Build() const;

  FilterBuilder &Reset();

  FilterBuilder &Pop();

  FilterBuilder &Remove(const FilterPtr filter);

private:
  FilterPtr filter_;
};

}

#endif 
