/*
* Created by suemi on 2017/1/18.
*/

#include "slog/filter/filter.h"

namespace slog {

Filter::~Filter() {}

bool Filter::Equals(const Filter &filter) const {
  return this==&filter;
}

}