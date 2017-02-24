/*
* Created by suemi on 2017/2/17.
*/

#include "slog/format/simple_layout.h"
#include "slog/base/fixed_buffer.h"

namespace slog {

SimpleLayout::SimpleLayout(const std::string &name) : Layout(name) {}

void SimpleLayout::Reformat(LogEvent &log) {

}

}

