/*
* Created by suemi on 2017/2/28.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "slog/utils/string_util.h"

using namespace slog;
using namespace std;

TEST(STRING_UTIL_TEST, TO_UPPER) {
  EXPECT_EQ('A', StringUtil::ToUpper('a'));
  EXPECT_EQ('~', StringUtil::ToUpper('~'));

  EXPECT_EQ("ABC M 4", StringUtil::ToUpper("abc m 4"));
}

TEST(STRING_UTIL_TEST, TO_LOWER) {
  EXPECT_EQ('a', StringUtil::ToLower('A'));
  EXPECT_EQ('~', StringUtil::ToLower('~'));

  EXPECT_EQ("abc m 4", StringUtil::ToLower("ABC M 4"));
}

TEST(STRING_UTIL_TEST, JOIN) {
  vector<string> tmp({"a", "b", "c"});
  EXPECT_EQ("a.b.c", StringUtil::Join(tmp.begin(), tmp.end(), "."));

  tmp.clear();
  EXPECT_EQ("", StringUtil::Join(tmp.begin(), tmp.end(), "."));
}

TEST(STRING_UTIL_TEST, CONVERT_INT) {
  EXPECT_EQ("1", StringUtil::ConvertInt(1));
  EXPECT_EQ("0", StringUtil::ConvertInt(0));
  EXPECT_EQ("-1", StringUtil::ConvertInt(-1));
}

TEST(STRING_UTIL_TEST, PARSE_BOOL) {
  bool tmp;
  EXPECT_EQ(true, StringUtil::ParseBool(tmp, "true"));
  EXPECT_TRUE(tmp);

  EXPECT_EQ(true, StringUtil::ParseBool(tmp, "false"));
  EXPECT_FALSE(tmp);

  EXPECT_EQ(false, StringUtil::ParseBool(tmp, "hh "));
}

TEST(STRING_UTIL_TEST, TOKENIZE) {
  vector<string> tmp;
  StringUtil::Tokenize("a.b.c", '.', back_inserter(tmp), true);
  EXPECT_THAT(vector<string>({"a", "b", "c"}), testing::ContainerEq(tmp));

  tmp.clear();

  StringUtil::Tokenize(".b..c.", '.', back_inserter(tmp), true);
  EXPECT_THAT(vector<string>({"", "b", "c"}), testing::ContainerEq(tmp));

  tmp.clear();

  StringUtil::Tokenize(".b..c.", '.', back_inserter(tmp), false);
  EXPECT_THAT(vector<string>({"", "b", "", "c"}), testing::ContainerEq(tmp));

  tmp.clear();
  StringUtil::Tokenize("", '.', back_inserter(tmp), false);
  EXPECT_THAT(vector<string>(), testing::ContainerEq(tmp));
}

TEST(STRING_UTIL_TEST, SPLIT) {
  EXPECT_THAT(StringUtil::Split("a.b.c", '.'), testing::ContainerEq(vector<string>({"a", "b", "c"})));

  EXPECT_THAT(StringUtil::Split(".b.c", '.'), testing::ContainerEq(vector<string>({"", "b", "c"})));

  EXPECT_THAT(StringUtil::Split("a.b.c.", '.'), testing::ContainerEq(vector<string>({"a", "b", "c"})));

  EXPECT_THAT(StringUtil::Split("", '.'), testing::ContainerEq(vector<string>()));
}

TEST(STRING_UTIL_TEST, LTRIM) {
  string tmp = " abc ";
  StringUtil::LTrim(tmp);
  EXPECT_EQ("abc ", tmp);

  tmp = "   ";
  StringUtil::LTrim(tmp);
  EXPECT_EQ("", tmp);

  tmp = "";
  StringUtil::LTrim(tmp);
  EXPECT_EQ("", tmp);
}

TEST(STRING_UTIL_TEST, RTRIM) {
  string tmp = " abc ";
  StringUtil::RTrim(tmp);
  EXPECT_EQ(" abc", tmp);

  tmp = "   ";
  StringUtil::RTrim(tmp);
  EXPECT_EQ("", tmp);

  tmp = "";
  StringUtil::RTrim(tmp);
  EXPECT_EQ("", tmp);
}

TEST(STRING_UTIL_TEST, TRIM) {
  string tmp = " abc ";
  StringUtil::Trim(tmp);
  EXPECT_EQ("abc", tmp);

  tmp = "   ";
  StringUtil::Trim(tmp);
  EXPECT_EQ("", tmp);

  tmp = "";
  StringUtil::Trim(tmp);
  EXPECT_EQ("", tmp);
}

TEST(STRING_UTIL_TEST, REMOVE_EMPTY) {
  vector<string> tmp({"a", "", "b", ""});
  StringUtil::RemoveEmpty(tmp);
  EXPECT_THAT(tmp, ::testing::ContainerEq(vector<string>({"a", "b"})));

  tmp = {"", "a", "", "", "c"};
  StringUtil::RemoveEmpty(tmp, 3);
  EXPECT_THAT(tmp, ::testing::ContainerEq(vector<string>({"", "a", "", "c"})));

}

