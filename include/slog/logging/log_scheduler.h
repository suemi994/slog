/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOG_SCHEDULER_H
#define SLOG_LOG_SCHEDULER_H

#include "slog/utils/no_copyable.h"

namespace slog {

class LogScheduler : public NoCopyable {
public:
  LogScheduler(int fulsh_interval);
};

}

#endif 
