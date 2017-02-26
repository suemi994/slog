/*
* Created by suemi on 2017/2/26.
*/

#ifndef SLOG_COUNT_LATCH_H
#define SLOG_COUNT_LATCH_H

#include <mutex>
#include "slog/utils/no_copyable.h"

namespace slog {

class CountDownLatch : public NoCopyable {
public:

  explicit CountDownLatch(int count);

  void Wait();

  void CountDown();

  int count() const;

private:
  mutable std::mutex mutex_;
  std::condition_variable cond_;
  int count_;
};

}

#endif 
