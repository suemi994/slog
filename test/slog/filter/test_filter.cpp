/*
* Created by suemi on 2017/3/6.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "slog/logging/log_event.h"
#include "slog/filter/filter.h"
#include "slog/filter/filter_factory.h"
#include "slog/utils/properties.h"

using namespace std;
using namespace slog;

TEST(FILTER_TEST, ACCEPT_ALL) {
  FilterPtr filter = make_shared<AcceptAllFilter>();

  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, DENY_ALL) {
  FilterPtr filter = make_shared<DenyAllFilter>();

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::ERROR)));
}


TEST(FILTER_TEST, LEVEL_MATCH) {
  FilterPtr filter = make_shared<LogLevelMatchFilter>(LogLevel::WARN);

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, LEVEL_RANGE) {
  FilterPtr filter = make_shared<LogLevelRangeFilter>(LogLevel::INFO, LogLevel::WARN);

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, AND_FILTER) {
  FilterPtr filter = make_shared<AndFilter>(
      FilterList({make_shared<LogLevelMatchFilter>(LogLevel::WARN), make_shared<LogLevelMatchFilter>(LogLevel::INFO)}));

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, BINARY_AND_FILTER) {
  FilterPtr filter = make_shared<BinaryAndFilter>(make_shared<LogLevelMatchFilter>(LogLevel::WARN),
                                                  make_shared<LogLevelMatchFilter>(LogLevel::INFO));

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::ACCEPT, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, NOT_FILTER) {
  FilterPtr filter = make_shared<NotFilter>(make_shared<AcceptAllFilter>());

  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::TRACE)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::DEBUG)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::INFO)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::WARN)));
  EXPECT_EQ(Filter::Result::DENY, filter->Decide(LogEvent(LogLevel::ERROR)));
}

TEST(FILTER_TEST, FILTER_FACTORY) {
  FilterPtr filter = FilterFactory::CreateInstance("AcceptAllFilter", Properties());
  EXPECT_FALSE(dynamic_cast<AcceptAllFilter *>(filter.get()) == nullptr);
}