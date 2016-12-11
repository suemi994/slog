/*
* Created by suemi on 2016/12/10.
*/
#include <algorithm>
#include <sstream>
#include <fstream>
#include "slog/utils/properties.h"

namespace slog {

namespace detail {

/**
 * 去除某一行开头处的所有空格
 * @param line
 */
void trimLeadSpace(std::string &line) {
  auto it = line.begin();
  for (; it != line.end(); it++) {
    if (!std::isspace(*it)) break;
  }
  line.erase(line.begin(), it);
}

/**
 * 去除字符串结尾处的连续空格
 * @param line
 */
void trimTailSpace(std::string &line) {
  auto it = line.rbegin();
  for (; it != line.rend(); it++) {
    if (!std::isspace(*it)) break;
  }
  line.erase(it.base(), line.end());
}

/**
 * 去除字符串边界的所有空格
 * @param line
 */
void trimBorderSpace(std::string &line) {
  trimLeadSpace(line);
  trimTailSpace(line);
}

}

Properties::Properties() {

}

Properties::Properties(std::istream &in) {
  Initialize(in);
}

Properties::Properties(const std::string &input_file) {
  if (input_file.empty()) return;
  std::ifstream in;
  in.open(input_file.c_str(), std::ios::binary);
  if (!in.good()) {
    std::cerr << "slog property file" << input_file << " can't get!" << std::endl;
    //TODO 使用log_macro记录文件读取失败日志
  }
  Initialize(in);
}

bool Properties::Exists(const std::string &key) const {
  return values_.find(key) != values_.end();
}

bool Properties::Exists(const char *key) const {
  return values_.find(key) != values_.end();
}

int Properties::Size() const {
  return values_.size();
}

const std::string &Properties::GetProperty(const std::string &key) const {
  return DoGetProperty(key);
}

const std::string &Properties::GetProperty(const char *key) const {
  return DoGetProperty(key);
}

const std::string &Properties::GetProperty(const std::string &key, const std::string &default_value) const {
  auto val = DoGetProperty(key);
  return val.empty() ? default_value : val;
}

std::vector<std::string> Properties::PropertyNames() const {
  std::vector<std::string> keys;
  std::for_each(values_.cbegin(), values_.cend(), [&keys](StringMap::iterator item) {
    keys.push_back(item->first);
  });
  return keys;
}

void Properties::SetProperty(const std::string &key, const std::string &value) {
  values_[key] = value;
}

void Properties::RemoveProperty(const std::string &key) {
  values_.erase(key);
}

Properties Properties::GetPropertySubset(const std::string &prefix) const {
  Properties res;
  const auto prefix_len = prefix.size();
  std::for_each(values_.cbegin(), values_.cend(), [&](StringMap::iterator it) {
    if (it->first.compare(0, prefix_len, prefix)) {
      res.SetProperty(it->first, it->second);
    }
  });
  return res;
}

bool Properties::GetInt(int &val, const std::string &key) const {
  return GetValue<int>(val, key);
}

bool Properties::GetBool(bool &val, const std::string &key) const {
  return GetValue<bool>(val, key);
}

bool Properties::GetLong(long &val, const std::string &key) const {
  return GetValue<long>(val, key);
}

bool Properties::GetFloat(float &val, const std::string &key) const {
  return GetValue<float>(val, key);
}

void Properties::Initialize(std::istream &in) {
  if (!in) return;
  std::string line;
  while (std::getline(in, line)) {
    detail::trimLeadSpace(line);
    const auto line_size = line.size();

    //该行是空行或是注释行
    if (line_size == 0 || line[0] == PROPERTIES_COMMENT_CHAR)
      continue;

    const std::string::difference_type sep = line.find("=");
    if (sep != std::string::npos) { //能找到=号的情形
      auto key = line.substr(0, sep);
      detail::trimTailSpace(key);

      auto value = line.substr(sep + 1);
      detail::trimBorderSpace(value);

      SetProperty(key, value);
    } else if (!line.compare(0, 7, "include") && line.size() > 7 + 2 && std::isspace(line[7])) {
      // include其他配置文件的情形
      auto include_path = line.substr(8);
      detail::trimBorderSpace(include_path);
      std::ifstream include_file;

      include_file.open(include_path.c_str(), std::ios::binary);
      if (!in.good()) {
        std::cerr << "slog property file" << include_path << " can't get!" << std::endl;
        //TODO 使用log_macro记录文件读取失败日志
        continue;
      }
      Initialize(include_file);
    }

  }
}

const std::string &Properties::DoGetProperty(const std::string &key) const {
  auto it = values_.find(key);
  return it == values_.end() ? std::string() : it->second;
}

template<typename T>
bool Properties::GetValue(T &val, const std::string &key) const {
  const std::string &str_val = DoGetProperty(key);
  if (str_val.empty()) return false;
  std::stringstream converter(str_val);
  T tmp_val;
  converter >> tmp_val;
  char c;
  if ((!converter) || (converter >> c)) {
    converter.setstate(std::ios::badbit);
    return false;
  }
  val = tmp_val;
  return true;
}

}