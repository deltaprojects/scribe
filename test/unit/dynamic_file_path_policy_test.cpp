#include <gtest/gtest.h>
#include "dynamic_file_path_policy.h"
#include "hostname.h"
#include "timestamp_helper.h"

struct DynamicFilePathPolicyWithRegularFiles : public ::testing::Test {
  DynamicFilePathPolicyWithRegularFiles()
    : category("my-category") {
    timestamp = TimestampHelper::createTimestamp(2010, 5, 10, 15, 22);
  }
  
  void assertThatDynamicPathsWorkWith(const std::string & aKey, const std::string & expectedValue) {
    DynamicFilePathPolicy policy("/tmp/\%" + aKey + "\%/\%" + aKey + "\%.log");
    EXPECT_EQ("/tmp/" + expectedValue + "/" + expectedValue + ".log", policy.fullPath(&timestamp, category));
    EXPECT_EQ("/tmp/" + expectedValue, policy.directoryPath(&timestamp, category));
    EXPECT_EQ(expectedValue + ".log", policy.fileName(&timestamp, category));
  }
  
  const std::string category;
  struct tm timestamp;
};

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_hostname) {
  assertThatDynamicPathsWorkWith("hostname", Hostname::getHostname());
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_category) {
  assertThatDynamicPathsWorkWith("category", "my-category");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_year) {
  assertThatDynamicPathsWorkWith("year", "2010");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_month) {
  assertThatDynamicPathsWorkWith("month", "05");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_day) {
  assertThatDynamicPathsWorkWith("day", "10");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_hour) {
  assertThatDynamicPathsWorkWith("hour", "15");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_dynamically_set_the_minute) {
  assertThatDynamicPathsWorkWith("minute", "22");
}

TEST_F(DynamicFilePathPolicyWithRegularFiles, should_throw_an_exception_if_the_key_does_not_exist) {
  DynamicFilePathPolicy policy("/tmp/\%fail\%.log");
  ASSERT_THROW(policy.fullPath(&timestamp, category), std::runtime_error);
}

TEST(DynamicFilePathPolicyWithHdfsFiles, should_return_a_valid_url) {
  struct tm timestamp = TimestampHelper::createTimestamp(2010, 5, 10, 15, 22);
  DynamicFilePathPolicy policy("hdfs://namenode:8020/\%category\%/\%year\%/\%month\%/\%day\%.log");
  EXPECT_EQ("hdfs://namenode:8020/my-category/2010/05/10.log", policy.fullPath(&timestamp, "my-category"));
}
