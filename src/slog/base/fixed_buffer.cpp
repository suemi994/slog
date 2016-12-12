/*
* Created by suemi on 2016/12/9.
*/

#include "slog/base/fixed_buffer.h"

namespace slog {

template<int SIZE>
FixedBuffer<SIZE>::FixedBuffer() : pos_(0) {
  set_cookie(&FixedBuffer<SIZE>::CookieStart);
}

template<int SIZE>
FixedBuffer<SIZE>::~FixedBuffer() {
  set_cookie(&FixedBuffer<SIZE>::CookieEnd);
}

template<int SIZE>
void FixedBuffer<SIZE>::Append(const char *buffer, size_t len) {
  auto to_write_len = avail() < len ? avail() : len;
  memcpy(current(), buffer, to_write_len);
  pos_ += to_write_len;
}

template<int SIZE>
void FixedBuffer::Forward(size_t len) {
  pos_ += len;
  if (pos_ > SIZE) pos_ = SIZE;
}

template<int SIZE>
void FixedBuffer<SIZE>::Reset() {
  pos_ = 0;
}

template<int SIZE>
std::string FixedBuffer<SIZE>::ToString() const {
  return std::string(data_, length());
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
  return data_ + pos_;
}

template<int SIZE>
int FixedBuffer<SIZE>::length() const {
  return pos_;
}

template<int SIZE>
int FixedBuffer<SIZE>::avail() const {
  return sizeof(data_) - pos_;
}

template<int SIZE>
void FixedBuffer<SIZE>::set_cookie(std::function<void()> cookie) {
  cookie_ = cookie;
}

template<int SIZE>
const char *FixedBuffer<SIZE>::end() const {
  return data_ + sizeof(data_);
}

template<int SIZE>
void FixedBuffer<SIZE>::CookieStart() {
  // Do Nothing
}

template<int SIZE>
void FixedBuffer<SIZE>::CookieEnd() {
  // Do Nothing
}

template
class FixedBuffer<detail::SMALL_BUFFER_SIZE>;

template
class FixedBuffer<detail::LARGE_BUFFER_SIZE>;
}
