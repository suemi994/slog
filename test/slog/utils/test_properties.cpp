/*
* Created by suemi on 2016/12/11.
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>
#include <vector>

#include "slog/utils/properties.h"

using namespace slog;
using namespace std;

TEST(PROPERTIES_TEST,EXISTS){
  Properties prop;
  EXPECT_EQ(false,prop.Exists("key"));
  prop.SetProperty("key","value");
  EXPECT_EQ(true,prop.Exists("key"));
}

TEST(PROPERTIES_TEST, PREFIX_EXIST) {
  Properties prop;
  prop.SetProperty("a.b.c", "d.d.e");

  EXPECT_TRUE(prop.ExistPrefix("a.b"));
  EXPECT_FALSE(prop.ExistPrefix("b.e"));
}

TEST(PROPERTIES_TEST,SIZE){
  Properties prop;
  EXPECT_EQ(0,prop.Size());
  prop.SetProperty("key","value");
  EXPECT_EQ(1,prop.Size());
}

TEST(PROPERTIES_TEST,GET_PROPERTIE){
  Properties prop;
  EXPECT_EQ(std::string(),prop.GetProperty("key"));
  EXPECT_EQ("value",prop.GetProperty("key","value"));
  prop.SetProperty("key","value");
  EXPECT_EQ("value",prop.GetProperty("key"));
}

TEST(PROPERTIES_TEST,PROPERTIE_NAMES){
  Properties prop;
  prop.SetProperty("a","b");
  prop.SetProperty("c","d");
  EXPECT_THAT(std::vector<std::string>({"c","a"}),::testing::ContainerEq(prop.PropertyNames()));
}

TEST(PROPERTIES_TEST, PROPPERTY_WITHOUT_SUFFIX) {
  Properties prop;
  prop.SetProperty("a", "b");
  prop.SetProperty("a.b", "c");
  prop.SetProperty("a.c", "d");
  prop.SetProperty("b", "e");

  auto tmp = prop.PropertyWithoutSuffix();

  EXPECT_TRUE(tmp.find("a") != tmp.end());
  EXPECT_TRUE(tmp.find("a.b") == tmp.end());
}

TEST(PROPERTY_TEST, PART_OF_PROPERTY_NAMES) {
  Properties prop;
  prop.SetProperty("a.b", "w");
  prop.SetProperty("a.b.c", "q");
  prop.SetProperty("a.c.d", "r");

  EXPECT_THAT(prop.PartOfPropertyNames(1), ::testing::ContainerEq(vector<string>({"b", "c"})));
  EXPECT_THAT(prop.PartOfPropertyNames(3), ::testing::ContainerEq(vector<string>()));
}

TEST(PROPERTIES_TEST,SET_PROPERTY){
  Properties prop;
  prop.SetProperty("key","value");
  EXPECT_EQ("value",prop.GetProperty("key"));
  prop.SetProperty("key","new_value");
  EXPECT_EQ("new_value",prop.GetProperty("key"));
}

TEST(PROPERTIES_TEST,REMOVE_PROPERTY){
  Properties prop;
  prop.SetProperty("key","value");
  prop.RemoveProperty("key");
  EXPECT_EQ(false,prop.Exists("key"));
}


TEST(PROPERTIES_TEST,GET_SUBSET){
  Properties prop;
  prop.SetProperty("a_1","b_1");
  prop.SetProperty("c_1","d_1");
  Properties subset = prop.GetPropertySubset("a_");
  EXPECT_EQ("b_1", subset.GetProperty("1"));
  EXPECT_EQ(1, subset.Size());
}

TEST(PROPERTIES_TEST,GET_BY_TYPE){
  Properties prop;

  prop.SetProperty("int","1");
  int int_val;
  prop.GetInt(int_val,"int");
  EXPECT_EQ(1,int_val);

  prop.SetProperty("bool", "true");
  bool bool_val = false;
  prop.GetBool(bool_val,"bool");
  EXPECT_EQ(true,bool_val);

  prop.SetProperty("float","1.2");
  float float_val;
  prop.GetFloat(float_val,"float");
  EXPECT_EQ(float(1.2),float_val);

  prop.SetProperty("long","12345678");
  long long_val;
  prop.GetLong(long_val,"long");
  EXPECT_EQ(12345678,long_val);

  EXPECT_EQ(false,prop.GetBool(bool_val,"boolean"));

  prop.SetProperty("key","value");
  EXPECT_EQ(false,prop.GetInt(int_val,"key"));

}

TEST(PROPERTIES_TEST,INITIALIZE) {

  Properties prop("/Users/suemi/Workspace/slog/test/slog/resources/slog.properties");
  int flush_interval;
  prop.GetInt(flush_interval,"slog.scheduler.flushInterval");
  EXPECT_EQ(3,flush_interval);
}


