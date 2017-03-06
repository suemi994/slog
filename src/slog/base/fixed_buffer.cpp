/*
* Created by suemi on 2016/12/9.
*/

#include "slog/base/fixed_buffer.h"

namespace slog {

template<size_t SIZE>
FixedBuffer<SIZE>::FixedBuffer() : pos_(0) {
  set_cookie(&FixedBuffer<SIZE>::CookieStart);
}

template<size_t SIZE>
FixedBuffer<SIZE>::~FixedBuffer() {
  set_cookie(&FixedBuffer<SIZE>::CookieEnd);
}

template<size_t SIZE>
size_t FixedBuffer<SIZE>::Append(const char *buffer, size_t len) {
  auto to_write_len = avail() < len ? avail() : len;
  memcpy(current(), buffer, to_write_len);
  pos_ += to_write_len;
  return to_write_len;
}

template<size_t SIZE>
size_t FixedBuffer<SIZE>::Forward(size_t len) {
  auto to_forward = avail() < len ? avail() : len;
  pos_ += to_forward;
  return to_forward;
}

template<size_t SIZE>
bool FixedBuffer<SIZE>::Insert(int dst, const char *data, size_t len) {
  if (dst < 0 || dst > pos_ || avail() < len) return false; // 容量不够或参数不合法时什么也不做
  char *start = dst + data_;
  std::memcpy(start + len, start, pos_ - dst);
  std::memcpy(start, data, len);
  pos_ += len;
  return true;
}

template<size_t SIZE>
void FixedBuffer<SIZE>::Reset() {
  pos_ = 0;
}

template<size_t SIZE>
std::string FixedBuffer<SIZE>::ToString() const {
  return std::string(data_, length());
}

template<size_t SIZE>
void FixedBuffer<SIZE>::Bezero() {
  ::bzero(data_, sizeof(data_));
}

template<size_t SIZE>
const char *FixedBuffer<SIZE>::data() const {
  return data_;
}


template<size_t SIZE>
char *FixedBuffer<SIZE>::current() const {
  return const_cast<char *>(data_) + pos_;
}

template<size_t SIZE>
size_t FixedBuffer<SIZE>::length() const {
  return pos_;
}

template<size_t SIZE>
size_t FixedBuffer<SIZE>::avail() const {
  if (sizeof(data_) < pos_) return 0;
  return sizeof(data_) - pos_;
}

template<size_t SIZE>
void FixedBuffer<SIZE>::set_cookie(std::function<void()> cookie) {
  cookie_ = cookie;
}

template<size_t SIZE>
const char *FixedBuffer<SIZE>::end() const {
  return data_ + sizeof(data_);
}

template<size_t SIZE>
void FixedBuffer<SIZE>::CookieStart() {
  // Do Nothing
}

template<size_t SIZE>
void FixedBuffer<SIZE>::CookieEnd() {
  // Do Nothing
}

template
class FixedBuffer<detail::SMALL_BUFFER_SIZE>;

template
class FixedBuffer<detail::LARGE_BUFFER_SIZE>;
}
