/*
* Created by suemi on 2017/2/17.
*/

#include <sstream>

#include "slog/format/simple_layout.h"
#include "slog/base/fixed_buffer.h"
#include "slog/logging/log_event.h"
#include "slog/utils/time_util.h"

namespace slog {

SimpleLayout::SimpleLayout(const std::string &name) : Layout(name) {}

SimpleLayout::SimpleLayout(const Properties &properties) : Layout(properties) {}

/*
 * 输出格式示例：[08:58:59:412][INFO] (main.cpp:35 test::A::print) - 服务器启动
 */
void SimpleLayout::Reformat(LogEvent &log) {
  auto &buf = log.buffer();
  std::ostringstream oss;

  oss << "[" << TimeUtil::Format(log.time(), "%F %T") << "]";
  oss << "[" << ToString(log.log_level()) << "]";
  Location &location = log.location();
  oss << "(" << location.source_file << ":" << location.line << " ";
  oss << location.method << ") - ";

  auto str = oss.str();
  buf.Insert(0, str.c_str(), str.length());

  log << '\n';
}

}

