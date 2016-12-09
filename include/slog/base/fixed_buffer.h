/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FIXED_BUFFER_H
#define SLOG_FIXED_BUFFER_H

#include <string>
#include <functional>
#include "slog/utils/no_copyable.h"

namespace slog {

template<int SIZE>
class FixedBuffer {
public:

  FixedBuffer();

  ~FixedBuffer();

  void Append(const char *buffer, size_t len);

  void Reset();

  std::string ToString() const;

  void Bezero();

  const char *data() const;

  char *current() const;

  int length() const;

  int avail() const;

  void set_cookie(std::function<void()> cookie);

private:
  const char* end() const ;
  std::function<void()> cookie_;

  static void CookieStart();
  static void CookieEnd();

  int pos_;
  char data_[SIZE];
};

namespace detail {
const int SMALL_BUFFER_SIZE = 1024;
const int LARGE_BUFFER_SIZE = 1024*4096;
}

}

#endif 
