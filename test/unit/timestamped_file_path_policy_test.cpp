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

TEST(TimestampedFilePathPolicy, should_return_that_it_is_timestamped) {
  TimestampedFilePathPolicy policy("/tmp", "subdirectory");
  EXPECT_FALSE(policy.isTimestamped());
}
