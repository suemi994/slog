/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_PROPERTIES_H
#define SLOG_PROPERTIES_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace slog {

/**
 * 解析properties文件后得到的对象，有助于读取配置
 * @author suemi
 */
class Properties {
  using StringMap = std::unordered_map<std::string, std::string>;
public:

  Properties();

  explicit Properties(std::istream &in);

  explicit Properties(const std::string &input_file);

  bool Exists(const std::string &key) const;

  bool ExistPrefix(const std::string& prefix);

  bool Exists(const char *key) const;

  size_t Size() const;

  const std::string &GetProperty(const std::string &key) const;

  const std::string &GetProperty(const char *key) const;

  const std::string &GetProperty(const std::string &key, const std::string &default_value) const;

  std::vector<std::string> PropertyNames() const;

  StringMap PropertyWithoutSuffix() const;

  std::vector<std::string> PartOfPropertyNames(unsigned int i, char sep = '.') const;

  void SetProperty(const std::string &key, const std::string &value);

  void RemoveProperty(const std::string &key);

  Properties GetPropertySubset(const std::string &prefix) const;

  bool GetInt(int &val, const std::string &key) const;

  bool GetBool(bool &val, const std::string &key) const;

  bool GetLong(long &val, const std::string &key) const;

  bool GetFloat(float &val, const std::string &key) const;

  StringMap GetRawValues() const;

  static const char PROPERTIES_COMMENT_CHAR;

private:

  void Initialize(std::istream &in);

  const std::string &DoGetProperty(const std::string &key) const;

  template<typename T>
  bool GetValue(T &val, const std::string &key) const;

  StringMap values_;

};

}

#endif 
