#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <sstream>
#include <cstring>
#include <unistd.h>

#include "slog/appender/appender_all.h"
#include "slog/logging/log_event.h"
#include "slog/logging/logger.h"
#include "slog/logging/log_scheduler.h"
#include "slog/base/configurator.h"

using namespace slog;
using namespace std;

int main() {
  chdir("../");

  std::shared_ptr<Configurator> conf = std::make_shared<PropertyConfigurator>("./test/slog/resources/slog.properties");
  conf->Configure();

  auto root = conf->root_logger();

  auto app = conf->loggers()["app"];

  auto scheduler = conf->scheduler();
  scheduler->Start();

  LogEvent log(LogLevel::INFO);
  log << "Hello,World! \n";
  root->Submit(log);

  app->Submit(log);

  scheduler->Stop();

}