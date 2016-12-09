/*
* Created by suemi on 2016/12/9.
*/

#include "slog/base/fixed_buffer.h"

namespace slog {

template<int SIZE>
FixedBuffer<SIZE>::FixedBuffer() : pos_(0) {

}

template<int SIZE>
void FixedBuffer<SIZE>::Append(const char *buffer, size_t len) {

}

template<int SIZE>
void FixedBuffer::Reset() {
  pos_ = 0;
}

template<int SIZE>
std::string FixedBuffer<SIZE>::ToString() const {
  return std::string(data_,length());
}

template<int SIZE>
void FixedBuffer<SIZE>::Bezero() {
  ::bzero(data_, sizeof(data_));
}

template<int SIZE>
const char *FixedBuffer<SIZE>::data() const {
  return data_;
}

template<int SIZE>
char *FixedBuffer<SIZE>::current() const {
  return data_+pos_;
}

template<int SIZE>
int FixedBuffer<SIZE>::length() const {
  return pos_;
}

template<int SIZE>
int FixedBuffer<SIZE>::avail() const {
  return sizeof(data_)-pos_;
}

template<int SIZE>
void FixedBuffer<SIZE>::set_cookie(std::function<void()> cookie) {
  cookie_ = cookie;
}

template<int SIZE>
const char *FixedBuffer<SIZE>::end() const {
  return data_+ sizeof(data_);
}

template<int SIZE>
void FixedBuffer<SIZE>::CookieStart() {
  // Do Nothing
}

template<int SIZE>
void FixedBuffer<SIZE>::CookieEnd() {
  // Do Nothing
}

}
