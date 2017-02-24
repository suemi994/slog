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
size_t FixedBuffer<SIZE>::Append(const char *buffer, size_t len) {
  auto to_write_len = avail() < len ? avail() : len;
  memcpy(current(), buffer, to_write_len);
  pos_ += to_write_len;
  return to_write_len;
}

template<int SIZE>
size_t FixedBuffer::Forward(size_t len) {
  auto to_forward = avail() < len ? avail() : len;
  pos_ += to_forward;
  return to_forward;
}

template<int SIZE>
bool FixedBuffer::Insert(int dst, const char *data, size_t len) {
  if(dst<0 || dst>pos_ || avail()<len) return false; // 容量不够或参数不合法时什么也不做
  char* start = dst + data_;
  std::memcpy(start+len,start,pos_-dst);
  std::memcpy(start,data,len);
  pos_ += len;
  return true;
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
