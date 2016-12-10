/*
* Created by suemi on 2016/12/10.
*/

#include <gtest/gtest.h>
#include <string>
#include "slog/utils/formatter.h"

using namespace slog;

TEST(FORMATTER_TEST, CHAR_INPUT) {
  Formatter fmt("%2c", 'a');
  EXPECT_EQ(" a", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, INT_INPUT) {
  int tmp = 43;
  Formatter fmt("%4d", tmp);
  auto s = std::string(fmt.data(), fmt.length());
  EXPECT_EQ("  43", s);
}

TEST(FORMATTER_TEST, UINT_INPUT) {
  unsigned int tmp = -1024;
  Formatter fmt("%u", tmp);
  EXPECT_EQ("4294966272", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, SHORT_INPUT) {
  short tmp = 10;
  Formatter fmt("%o", tmp);
  EXPECT_EQ("12", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, USHORT_INPUT) {
  unsigned short tmp = -10;
  Formatter fmt("%u", tmp);
  EXPECT_EQ("65526", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, LONG_INPUT) {
  long tmp = -123456789;
  Formatter fmt("%10d", tmp);
  EXPECT_EQ("-123456789", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, ULONG_INPUT) {
  unsigned long tmp = -123456789;
  Formatter fmt("%10u", tmp);
  EXPECT_EQ("4171510507", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, LONG_LONG_INPUT) {
  long long tmp = -123456789;
  Formatter fmt("%10d", tmp);
  EXPECT_EQ("-123456789", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, ULONG_LONG_INPUT) {
  unsigned long long tmp = -123456789;
  Formatter fmt("%10u", tmp);
  EXPECT_EQ("4171510507", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, FLOAT_INPUT) {
  float tmp = 1.123;
  Formatter fmt("%1.2lf", tmp);
  EXPECT_EQ("1.12", std::string(fmt.data(), fmt.length()));
  fmt = Formatter("%e",tmp);
  EXPECT_EQ("1.123000e+00", std::string(fmt.data(), fmt.length()));
}

TEST(FORMATTER_TEST, DOUBLE_INPUT) {
  double tmp = 1.123456789;
  Formatter fmt("%1.2lf", tmp);
  EXPECT_EQ("1.12", std::string(fmt.data(), fmt.length()));
  fmt = Formatter("%e",tmp);
  EXPECT_EQ("1.1234567e+00", std::string(fmt.data(), fmt.length()));
}

