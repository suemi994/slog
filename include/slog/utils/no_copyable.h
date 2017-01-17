/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_NO_COPYABLE_H
#define SLOG_NO_COPYABLE_H

namespace slog {

class NoCopyable {
protected:
  NoCopyable() = default;

  virtual ~NoCopyable() = default;

  NoCopyable(const NoCopyable &) = delete;

  NoCopyable &operator=(const NoCopyable &) = delete;
};

}

#endif 
