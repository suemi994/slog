/*
* Created by suemi on 2017/3/6.
*/

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>

#include "slog/appender/all.h"
#include "slog/utils/properties.h"

using namespace slog;
using namespace std;

int main() {

  FixedBuffer<detail::LARGE_BUFFER_SIZE> buffer;

  const char *str = "Hello,World!\n";
  buffer.Append(str, strlen(str));

  auto console_appender = AppenderFactory::CreateInstance("ConsoleAppender", Properties());
  console_appender->Append(buffer);

  chdir("../");
  Properties prop;
  prop.SetProperty("createDirs", "true");
  prop.SetProperty("path", "./test/slog/resources/log/log.txt");
  auto file_appender = AppenderFactory::CreateInstance("FileAppender", prop);
  file_appender->Append(buffer);
  return 0;
}