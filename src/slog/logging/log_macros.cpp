/*
* Created by suemi on 2017/2/27.
*/

#include "slog/logging/log_macros.h"

namespace slog {

std::string MethodName(const std::string& pretty_function){
  size_t end = pretty_function.find("(");
  size_t start = pretty_function.find(" ")+1;
  return pretty_function.substr(start,end-start);
}

}