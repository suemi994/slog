/*
* Created by suemi on 2017/2/17.
*/

#include "slog/format/layout.h"

namespace slog {

Layout::Layout(const std::string &name) :name_(name) {}

Layout::~Layout() {}

const std::string &Layout::name() const {
  return name_;
}

}