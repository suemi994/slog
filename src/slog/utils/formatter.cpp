/*
* Created by suemi on 2016/12/10.
*/

#include <type_traits>
#include <cstdio>
#include <assert.h>
#include "slog/utils/formatter.h"

namespace slog {
template<typename T>
Formatter::Formatter(const char *fmt, T val) {
  static_assert(std::is_arithmetic<T>::value== true,"Formatter just for arithmetic type!"); //确保T为基本类型
  length_ = snprintf(buf_, sizeof(buf_),fmt,val);
  assert(static_cast<size_t>(length_)< sizeof(buf_));
}

int Formatter::length() const {
  return length_;
}

const char * Formatter::data() const {
  return buf_;
}

template Formatter::Formatter(const char *fmt, char);
template Formatter::Formatter(const char *fmt, short);
template Formatter::Formatter(const char *fmt, unsigned short);
template Formatter::Formatter(const char *fmt, int);
template Formatter::Formatter(const char *fmt, unsigned int);
template Formatter::Formatter(const char *fmt, long);
template Formatter::Formatter(const char *fmt, unsigned long);
template Formatter::Formatter(const char *fmt, long long);
template Formatter::Formatter(const char *fmt, unsigned long long);
template Formatter::Formatter(const char *fmt, float);
template Formatter::Formatter(const char *fmt, double);
}