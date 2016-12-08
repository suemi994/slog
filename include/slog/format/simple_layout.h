/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_SIMPLE_LAYOUT_H
#define SLOG_SIMPLE_LAYOUT_H

#include "slog/format/layout.h"

namespace slog {

class SimpleLayout : public Layout {
public:
  SimpleLayout();

  virtual ~SimpleLayout() override;

  virtual void Reformat(LogEvent &log) override;
};

}

#endif 
