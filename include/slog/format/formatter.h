/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_FORMATTER_H
#define SLOG_FORMATTER_H

namespace slog {

/**
 * 格式化基本类型的辅助类
 * @author suemi
 */
class Formatter {
public:
  template<typename T>
  Formatter(const char* fmt, T val);

  int length() const ;
  const char* data() const ;
private:
  int length_;
  char buf_[32];
};

}

#endif 
