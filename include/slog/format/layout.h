/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LAYOUT_H
#define SLOG_LAYOUT_H

#include <string>
#include "slog/utils/no_copyable.h"

namespace slog {

class LogEvent;

class Properties;

class Layout : public NoCopyable {
public:
  Layout() = default;

  Layout(const Properties &properties);

  virtual ~Layout();

  virtual void Reformat(LogEvent &log) = 0;

  const std::string &name() const;
};

}

#endif 
