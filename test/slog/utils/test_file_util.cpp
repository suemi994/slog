/*
* Created by suemi on 2017/2/28.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "slog/utils/file_util.h"

using namespace std;
using namespace slog;

TEST(FILE_UTIL_TEST, EXISTS) {
  EXPECT_TRUE(FileUtil::Exists("."));
  EXPECT_TRUE(FileUtil::Exists("../"));
  EXPECT_FALSE(FileUtil::Exists("./hehe"));
}

TEST(FILE_UTIL_TEST, LIST_DIR) {
  if (!FileUtil::Exists("./test")) FileUtil::MakeDirectory("./test");
  if (!FileUtil::Exists("./test/log")) FileUtil::MakeDirectory("./test/log");
  if (!FileUtil::Exists("./test/test.txt")) FileUtil::Touch("./test/test.txt");

  EXPECT_THAT(FileUtil::ListDir("./test/test.txt"), ::testing::ContainerEq(vector<string>()));

  EXPECT_THAT(FileUtil::ListDir("./test"), ::testing::ContainerEq(vector<string>({"./test/log", "./test/test.txt"})));
}

TEST(FILE_UTIL_TEST, TOUCH) {
  FileUtil::Touch("./test.txt");
  EXPECT_TRUE(FileUtil::Exists("./test.txt"));
}

TEST(FILE_UTIL_TEST, DELETE) {
  if (!FileUtil::Exists("./test.txt"))
    FileUtil::Touch("./test.txt");
  EXPECT_TRUE(FileUtil::Exists("./test.txt"));

  FileUtil::Delete("./test.txt");

  EXPECT_FALSE(FileUtil::Exists("./test.txt"));
  if (!FileUtil::Exists("./test"))
    FileUtil::MakeDirectory("./test");
  EXPECT_TRUE(FileUtil::IsDir("./test"));
  FileUtil::Delete("./test");
  EXPECT_FALSE(FileUtil::Exists("./test"));
}

TEST(FILE_UTIL_TEST, RENAME) {
  if (!FileUtil::Exists("./test.txt"))
    FileUtil::Touch("./test.txt");
  FileUtil::Rename("./test.txt", "./rename.txt");
  EXPECT_FALSE(FileUtil::Exists("./test.txt"));
  EXPECT_TRUE(FileUtil::Exists("./rename.txt"));

  FileUtil::Delete("./rename.txt");
}

TEST(FILE_UTIL_TEST, IS_DIR) {
  EXPECT_TRUE(FileUtil::IsDir("."));
  if (!FileUtil::Exists("./test.txt"))
    FileUtil::Touch("./test.txt");
  EXPECT_FALSE(FileUtil::IsDir("./test.txt"));
  EXPECT_FALSE(FileUtil::IsDir("./hehe"));
}

TEST(FILE_UTIL_TEST, PARENT_DIR) {
  EXPECT_EQ("/Users/suemi/Workspace/slog", FileUtil::ParentDir("/Users/suemi/Workspace/slog/cmake-build-debug/"));
  EXPECT_EQ("/Users/suemi/Workspace/slog", FileUtil::ParentDir("/Users/suemi/Workspace/slog/cmake-build-debug"));
  EXPECT_EQ("/Users/suemi/Workspace/slog/cmake-build-debug",
            FileUtil::ParentDir("/Users/suemi/Workspace/slog/cmake-build-debug/test.txt"));
}

TEST(FILE_UTIL_TEST, CURRENT_DIR) {
  chdir("/Users/suemi/Workspace/slog");
  EXPECT_EQ("/Users/suemi/Workspace/slog", FileUtil::CurrentDir());
}

TEST(FILE_UTIL_TEST, EXPAND_RELATIVE_PATH) {
  chdir("/Users/suemi/Workspace/slog");
  EXPECT_EQ("/Users/suemi/Workspace/slog/cmake-build-debug", FileUtil::ExpandRelativePath("./cmake-build-debug"));
  EXPECT_EQ("/Users/suemi/Workspace/slog/cmake-build-debug", FileUtil::ExpandRelativePath("../slog/cmake-build-debug"));
  EXPECT_EQ("/", FileUtil::ExpandRelativePath("../../../.././.././../"));
}

TEST(FILTE_UTIL_TEST, MAKE_DIRECTORY) {
  if (FileUtil::Exists("./test"))
    FileUtil::Delete("./test");
  EXPECT_FALSE(FileUtil::Exists("./test"));

  FileUtil::MakeDirectory("./test");

  EXPECT_TRUE(FileUtil::Exists("./test") && FileUtil::IsDir("./test"));

  FileUtil::Delete("./test");
}

TEST(FILTE_UTIL_TEST, MKDIRS) {
  if (FileUtil::Exists("./test"))
    FileUtil::Delete("./test");
  EXPECT_FALSE(FileUtil::Exists("./test"));
  FileUtil::MakeDirs("./test/log/log.txt");
  EXPECT_TRUE(FileUtil::IsDir("./test/log"));
  FileUtil::Delete("./test");
}

TEST(FILE_UTIL_TEST, ROLL_OVER) {
  if (FileUtil::Exists("./log")) FileUtil::Delete("./log");
  FileUtil::MakeDirectory("./log");
  FileUtil::Touch("./log/log.1");

  FileUtil::RollOverFiles("./log/log", 2);

  EXPECT_TRUE(FileUtil::Exists("./log/log.2"));
  EXPECT_FALSE(FileUtil::Exists("./log/log.1"));
  FileUtil::Delete("./log");
}