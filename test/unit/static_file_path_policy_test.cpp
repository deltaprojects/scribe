//  Copyright (c) 2010 Delta Projects AB
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// See accompanying file LICENSE or visit the Scribe site at:
// http://developers.facebook.com/scribe/
//
// @author Johan Stille

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

TEST(StaticFilePathPolicy, should_return_that_it_is_not_timestamped) {
  StaticFilePathPolicy policy("/tmp", "subdirectory");
  EXPECT_TRUE(policy.isTimestamped());
}
