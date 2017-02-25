/*
* Created by suemi on 2017/2/14.
*/

#ifndef SLOG_STRING_UTIL_H
#define SLOG_STRING_UTIL_H

#include <string>
#include <vector>

namespace slog {

class StringUtil {
public:
  static std::string ToUpper(const std::string &str);

  static char ToUpper(char ch);

  static std::string ToLower(const std::string &str);

  static char ToLower(char ch);

  template<class Iterator>
  static std::string Join(Iterator first, Iterator last, const std::string &sep);

  static std::string ConvertInt(int val);

  static int ParseInt(const std::string &str);

  template<class OutputIterator>
  static void Tokenize(const std::string &str, char ch, OutputIterator out, bool collapse);

  static std::vector<std::string> Split(const std::string &str, char sep);

  static void LTrim(std::string &str);

  static void RTrim(std::string &str);

  static void Trim(std::string &str);

  static void RemoveEmpty(std::vector<std::string> &components, unsigned int reserve = 0);

  static bool Environment(std::string& dst,const std::string& key);
};

}

#endif 
