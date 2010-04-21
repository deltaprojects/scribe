#include <gtest/gtest.h>
#include "timestamped_file_path_policy.h"
#include "timestamp_helper.h"

struct TimestampedFilePathPolicyWithRegularFiles : public ::testing::Test {
  TimestampedFilePathPolicyWithRegularFiles()
    : category("category")
    , policy("/tmp", "subdirectory") {
    timestamp = TimestampHelper::createTimestamp(2010, 5, 10);
  }
  
  const std::string category;
  TimestampedFilePathPolicy policy;
  struct tm timestamp;
};

TEST_F(TimestampedFilePathPolicyWithRegularFiles, should_build_a_timestamped_file_name) {
  EXPECT_EQ("category-2010-05-10", policy.fileName(&timestamp, category));
}

TEST_F(TimestampedFilePathPolicyWithRegularFiles, should_return_a_proper_directory_path) {
  EXPECT_EQ("/tmp/category/subdirectory", policy.directoryPath(&timestamp, category));
}

TEST_F(TimestampedFilePathPolicyWithRegularFiles, should_have_a_full_path_consisting_of_the_directory_path_and_the_file_name) {
  EXPECT_EQ("/tmp/category/subdirectory/category-2010-05-10", policy.fullPath(&timestamp, category));
}
