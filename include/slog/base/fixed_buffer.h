/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FIXED_BUFFER_H
#define SLOG_FIXED_BUFFER_H

#include <string>
#include <functional>
#include "slog/utils/no_copyable.h"

namespace slog {

template<size_t SIZE>
class FixedBuffer {
public:

  FixedBuffer();

  ~FixedBuffer();

  size_t Append(const char *buffer, size_t len);

  size_t Forward(size_t len);

  bool Insert(int dst,const char* data, size_t len);

  void Reset();

  std::string ToString() const;

  void Bezero();

  const char *data() const;

  char *current() const;

  size_t length() const;

  size_t avail() const;

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
const size_t SMALL_BUFFER_SIZE = 1024;
const size_t LARGE_BUFFER_SIZE = 1024*4096;
}

}

#endif 
