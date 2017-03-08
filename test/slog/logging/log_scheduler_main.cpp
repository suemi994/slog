#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>

#include "slog/appender/all.h"
#include "slog/utils/properties.h"
#include "slog/logging/log_scheduler.h"

using namespace slog;
using namespace std;

int main() {
  chdir("../");

  auto console_appender = AppenderFactory::CreateInstance("ConsoleAppender", Properties());

  Properties prop;
  prop.SetProperty("createDirs", "true");
  prop.SetProperty("path", "./test/slog/resources/log/log.txt");
  auto file_appender = AppenderFactory::CreateInstance("FileAppender", prop);

  LogScheduler scheduler(3);
  //scheduler.AddAppender(console_appender);
  scheduler.AddAppender(file_appender);

  scheduler.Start();

  vector<thread> workers;

  for (int i = 0; i < 4; ++i) {
    workers.push_back(thread([&scheduler]() {
      stringstream ss;
      for (int j = 0; j < 10000; ++j) {
        ss.str(std::string());
        ss.clear();
        ss << this_thread::get_id() << " : Hello,World! " << j << "     \n";
        auto str = ss.str();
        scheduler.Append(str.c_str(), str.length());
        //this_thread::sleep_for(chrono::microseconds(rand()%10));
      }
    }));
  }

  for (auto &worker:workers) worker.join();
  scheduler.Stop();

}