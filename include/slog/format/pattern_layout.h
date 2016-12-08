/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_PATTERN_LAYOUT_H
#define SLOG_PATTERN_LAYOUT_H

#include "slog/format/layout.h"

namespace slog {

class PatternLayout : public Layout {
public:
  PatternLayout();

  virtual ~PatternLayout() override;

  virtual void Reformat(LogEvent &log) override;
};

}

#endif 
