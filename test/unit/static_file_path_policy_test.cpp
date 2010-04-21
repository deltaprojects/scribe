#include <gtest/gtest.h>
#include "static_file_path_policy.h"

struct StaticFilePathPolicyWithRegularFiles : public ::testing::Test {
  StaticFilePathPolicyWithRegularFiles()
    : category("category")
    , now(NULL)
    , policy("/tmp", "subdirectory") {}
  
  const std::string category;
  struct tm * now;
  StaticFilePathPolicy policy;
};

TEST_F(StaticFilePathPolicyWithRegularFiles, should_always_return_the_category_as_the_file_name) {
  EXPECT_EQ(category, policy.fileName(now, category));
}

TEST_F(StaticFilePathPolicyWithRegularFiles, should_return_a_proper_directory_path) {
  EXPECT_EQ("/tmp/category/subdirectory", policy.directoryPath(now, category));
}

TEST_F(StaticFilePathPolicyWithRegularFiles, should_have_a_full_path_consisting_of_the_directory_path_and_the_file_name) {
  EXPECT_EQ("/tmp/category/subdirectory/category", policy.fullPath(now, category));
}

struct StaticFilePathPolicyWithHdfsFiles : public ::testing::Test {
  StaticFilePathPolicyWithHdfsFiles()
    : category("category")
    , now(NULL)
    , policy("hdfs://namenode:8020/tmp", "subdirectory") {}
  
  const std::string category;
  struct tm * now;
  StaticFilePathPolicy policy;
};

TEST_F(StaticFilePathPolicyWithHdfsFiles, should_return_full_url_as_a_directory_path) {
  EXPECT_EQ("hdfs://namenode:8020/tmp/category/subdirectory", policy.directoryPath(now, category));
}

TEST_F(StaticFilePathPolicyWithHdfsFiles, should_have_a_full_url_consisting_of_the_directory_path_and_the_file_name) {
  EXPECT_EQ("hdfs://namenode:8020/tmp/category/subdirectory/category", policy.fullPath(now, category));
}
