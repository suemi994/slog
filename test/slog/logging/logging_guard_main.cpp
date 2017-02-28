/*
* Created by suemi on 2017/2/28.
*/

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include "slog/logging/log_guard.h"

using namespace slog;
using namespace std;

int main() {
  vector<thread> tasks;
  auto ptr = LogGuard::Instance();
  for (int i = 0; i < 5; ++i) {
    tasks.push_back(thread([ptr]() {
      ostringstream oss;
      for (int j = 0; j < 3; ++j) {
        this_thread::sleep_for(std::chrono::microseconds(rand() % 10));

        oss.str("");
        oss << this_thread::get_id() << ": the " << j << "th print!";
        LogGuard::Instance()->Warn(oss.str());

        string tmp = (LogGuard::Instance() == ptr ? "True" : "False");
        cout << "guard address equals: " + tmp + "\n";
      }
    }));
  }

  for (auto &task:tasks)
    task.join();

  try {
    LogGuard::Instance()->Error("throw a exception!", true);
  } catch (runtime_error &e) {
    cout << "Caught Error: " << e.what() << endl;
  }


  return 0;

}