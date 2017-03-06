/*
* Created by suemi on 2016/12/12.
*/

#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "slog/logging/log_event.h"

using namespace slog;
using namespace std;


TEST(LOG_EVENT_TEST, MESSAGE) {
  LogEvent log(LogLevel::DEBUG);
  log << "ssss";
  EXPECT_EQ("ssss", log.message());
}

TEST(LOG_EVENT_TEST, RESET) {
  LogEvent log(LogLevel::DEBUG);
  log << "ssss";
  log.Reset();
  EXPECT_EQ(0, log.buffer().length());
}

TEST(LOG_EVENT_TEST, APPEND_BOOL) {
  LogEvent log(LogLevel::DEBUG);
  log << true;
  EXPECT_EQ("1", log.message());
  log.Reset();
  log << false;
  EXPECT_EQ("0", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_SHORT) {
  LogEvent log(LogLevel::DEBUG);
  short tmp = -10;
  log << tmp;
  EXPECT_EQ("-10", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_USHORT) {
  LogEvent log(LogLevel::DEBUG);
  unsigned short tmp = 10;
  log << tmp;
  EXPECT_EQ("10", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_INT) {
  LogEvent log(LogLevel::DEBUG);
  int tmp = -512;
  log << tmp;
  EXPECT_EQ("-512", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_UINT) {
  LogEvent log(LogLevel::DEBUG);
  unsigned int tmp = 512;
  log << tmp;
  EXPECT_EQ("512", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_DOUBLE) {
  LogEvent log(LogLevel::DEBUG);
  double tmp = -2.12345678;
  log << tmp;
  EXPECT_EQ("-2.12345678", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_CONST_CHAR_PTR) {
  LogEvent log(LogLevel::DEBUG);
  log << "word";
  EXPECT_EQ("word", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_STRING) {
  LogEvent log(LogLevel::DEBUG);
  string s = "word";
  log << s;
  EXPECT_EQ("word", log.message());
}

TEST(LOG_EVENT_TEST, APPEND_BUFFER) {
  LogEvent log1(LogLevel::DEBUG);
  LogEvent log2(LogLevel::DEBUG);
  log2 << "word";
  log1 << log2.buffer();
  EXPECT_EQ("word", log1.message());
}

TEST(LOG_EVENT_TEST, APPEND_LOG) {
  LogEvent log1(LogLevel::DEBUG);
  LogEvent log2(LogLevel::DEBUG);
  log2 << "word";

  log1 << log2;
  EXPECT_EQ("word", log1.message());

  log1.Reset();
  log1 << Formatter("%2c", 'a');
  EXPECT_EQ(" a", log1.message());
}

TEST(LOG_EVENT_TEST, APPEND_FIXED_LEN) {
  LogEvent log(LogLevel::DEBUG);
  const char *str = "words";
  log.Append(str, 4);
  EXPECT_EQ("word", log.message());
}




