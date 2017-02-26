/*
* Created by suemi on 2017/2/26.
*/

#include "slog/utils/count_down_latch.h"

namespace slog {

using namespace std;

CountDownLatch::CountDownLatch(int count) : count_(count){}

void CountDownLatch::Wait() {
  unique_lock<mutex> lock(mutex_);
  cond_.wait(lock,[this]{
    return count_ <= 0;
  });
}

void CountDownLatch::CountDown() {
  unique_lock<mutex> lock(mutex_);
  --count_;
  if(count_==0) cond_.notify_all();
}

int CountDownLatch::count() const {
  unique_lock<mutex> lock(mutex_);
  return count_;
}

}