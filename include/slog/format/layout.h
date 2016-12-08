/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LAYOUT_H
#define SLOG_LAYOUT_H

#include "slog/utils/no_copyable.h"

namespace slog {

class LogEvent;

class Layout : public NoCopyable{
public:
  Layout() = default;
  virtual ~Layout();

  virtual void Reformat(LogEvent& log);
};

}

#endif 
