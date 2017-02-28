/*
* Created by suemi on 2017/2/28.
*/

#include <iostream>
#include "slog/utils/time_util.h"

using namespace slog;
using namespace std;

int main() {

  auto timestamp = TimeUtil::Now();
  cout << TimeUtil::Seconds(timestamp) << "s " << endl;
  cout << TimeUtil::MicroSeconds(timestamp) << "ms " << endl;
  cout << TimeUtil::MilliSeconds(timestamp) << "us " << endl;

  cout << TimeUtil::Format(timestamp, "%F %T") << endl;

  cout << TimeUtil::Format(TimeUtil::AddSeconds(timestamp, 60), "%F %T") << endl;

  auto time = TimeUtil::LocalTime(timestamp);
  cout << time.tm_year << endl;

  cout << TimeUtil::Format(TimeUtil::FromLocalTime(&time), "%F %T") << endl;
  return 0;
}