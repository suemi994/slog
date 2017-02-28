/*
* Created by suemi on 2017/2/28.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "slog/logging/log_level.h"

using namespace std;
using namespace slog;

TEST(LOG_LEVEL, TO_STRING) {
  EXPECT_EQ("TRACE", ToString(LogLevel::TRACE));
  EXPECT_EQ("INFO", ToString(LogLevel::INFO));
  EXPECT_EQ("DEBUG", ToString(LogLevel::DEBUG));
  EXPECT_EQ("WARN", ToString(LogLevel::WARN));
  EXPECT_EQ("ERROR", ToString(LogLevel::ERROR));
}

TEST(LOG_LEVEL, FROM_STRING) {
  EXPECT_EQ(LogLevel::TRACE, FromString("trace"));

  EXPECT_THROW(FromString("traced"), invalid_argument);
}