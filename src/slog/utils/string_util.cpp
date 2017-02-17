/*
* Created by suemi on 2017/2/14.
*/

#include <cstring>
#include <cctype>
#include <vector>
#include <functional>
#include <algorithm>
#include "slog/utils/string_util.h"

namespace slog {

std::string StringUtil::ToUpper(const std::string &str) {
  std::string res;
  using func = int (*)(int);
  std::transform(str.begin(), str.end(), std::back_inserter(res), (func) std::toupper);
  return res;
}

char StringUtil::ToUpper(char ch) {
  return (char) std::toupper(ch);
}

std::string StringUtil::ToLower(const std::string &str) {
  std::string res;
  using func = int (*)(int);
  std::transform(str.begin(), str.end(), std::back_inserter(res), (func) std::tolower);
  return res;
}

char StringUtil::ToLower(char ch) {
  return (char) std::tolower(ch);
}

template<class Iterator>
std::string StringUtil::Join(Iterator first, Iterator last, const std::string &sep) {
  std::string res;
  if (first != last) res = *first++;
  for (; first != last; ++first) {
    res += sep;
    res += *first;
  }
  return res;
}

std::string StringUtil::ConvertInt(int val) {
  return std::to_string(val);
}

int StringUtil::ParseInt(const std::string &str) {
  return std::stoi(str);
}

template<class OutputIterator>
void StringUtil::Tokenize(const std::string &str, char ch, OutputIterator out, bool collapse) {
  auto first = str.begin();
  auto last = first;
  for(;last!=str.end();++last){
    if(*last==ch){
      *out++ = std::string(first,last);
      if(collapse)
        for(;last+1!=str.end() && *last==ch;++last);
      first = last+1;
    }
  }
  if(first!=last) *out = std::string(first,last);
}

std::vector<std::string> StringUtil::Split(const std::string &str, char sep) {
  std::vector<std::string> res;
  for(auto it=str.begin();it!=str.end();){
    auto tmp = std::find_if(it,str.end(),[](char ch){
      return ch==sep;
    });
    res.push_back(std::string(it,tmp));
    it = tmp;
    if(it!=str.end()) ++it;
  }
  return res;
}

void StringUtil::LTrim(std::string &str) {
  str.erase(str.begin(),std::find_if_not(str.begin(),str.end(),std::ptr_fun(std::isspace)));
}

void StringUtil::RTrim(std::string &str) {
  str.erase(std::find_if_not(str.rbegin(),str.rend(),std::ptr_fun(std::isspace)).base(),str.end());
}

void StringUtil::Trim(std::string &str) {
  LTrim(str);
  RTrim(str);
}

void StringUtil::RemoveEmpty(std::vector<std::string> &components, unsigned int reserve) {
  components.erase(std::remove_if(components.begin()+reserve,components.end(),[](const std::string& str){
    return str.empty();
  }),components.end());
}

using VectorStringIterator = std::vector<std::string,std::allocator<std::string>>::iterator;
template std::string StringUtil::Join<VectorStringIterator>(VectorStringIterator first, VectorStringIterator last, const std::string &sep);

using VectorInsertIterator = std::back_insert_iterator<std::vector<std::string,std::allocator<std::string>> >;
template void StringUtil::Tokenize<VectorInsertIterator >(const std::string &str, char ch, VectorInsertIterator out, bool collapse);

}