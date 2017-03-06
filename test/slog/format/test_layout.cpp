/*
* Created by suemi on 2017/2/28.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>

#include "slog/format/format.h"
#include "slog/logging/log_event.h"
#include "slog/utils/properties.h"

using namespace std;
using namespace slog;

#define MAKE_LOG(log) \
LogEvent log(LogLevel::INFO); \
log.Locate(__FILE__,"test",__LINE__,this_thread::get_id()); \



TEST(LAYOUT_TEST, SIMPLE_LAYOUT) {
  SimpleLayout layout;
  MAKE_LOG(log);
  log << " Hello,World! ";
  layout.Reformat(log);

  string dst = "[1970-01-01 08:00:00][INFO](/Users/suemi/Workspace/slog/test/slog/format/test_layout.cpp:23 test) -  Hello,World! \n";
  EXPECT_EQ(dst, log.message());
}

TEST(LAYOUT_TEST, PATTERN_LAYOUT) {
  PatternLayout layout("%d - %p - %c %m %n");
  MAKE_LOG(log);
  log << " Hello,World! ";
  layout.Reformat(log);

  EXPECT_EQ("1970-01-01 08:00:00 - INFO - root  Hello,World!  \n", log.message());
}

TEST(LAYOUT_TEST, LAYOUT_FACTORY) {
  auto layout = LayoutFactory::CreateInstance("SimpleLayout", Properties());
  EXPECT_FALSE(dynamic_cast<SimpleLayout *>(layout.get()) == nullptr);
}



