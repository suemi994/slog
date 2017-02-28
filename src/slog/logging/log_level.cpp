/*
* Created by suemi on 2017/2/27.
*/

#include "slog/logging/log_level.h"

namespace slog {

std::string ToString(LogLevel level){
  return ((const char* []){"TRACE","DEBUG","INFO","WARN","ERROR"})[static_cast<int>(level)];
}

LogLevel FromString(const std::string& level){
  static std::unordered_map<std::string,LogLevel> hash_table = {
      {"TRACE",LogLevel::TRACE},
      {"DEBUG",LogLevel::DEBUG},
      {"INFO",LogLevel::INFO},
      {"WARN",LogLevel::WARN},
      {"ERROR",LogLevel::ERROR}
  };
  std::string tmp;
  tmp.resize(level.length());
  std::transform(level.begin(), level.end(), tmp.begin(), [](char ch) {
    return std::toupper(ch);
  });

  auto it = hash_table.find(tmp);
  if(it!=hash_table.end())
    return it->second;

  throw std::invalid_argument("LogLevel should vary from TRACE to ERROR!");
}

}