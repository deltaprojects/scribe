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
#include "file_path_policy_factory.h"
#include "static_file_path_policy.h"
#include "dynamic_file_path_policy.h"
#include "timestamped_file_path_policy.h"

TEST(FilePathPolicyFactory, should_return_a_dynamic_file_path_policy_if_the_path_contains_percentage_characters) {
  boost::shared_ptr<FilePathPolicy> policy = FilePathPolicyFactory::createFilePathPolicy("/tmp/\%category\%.log", "", false);
  if (!boost::dynamic_pointer_cast<DynamicFilePathPolicy>(policy)) {
    FAIL();
  }
}

TEST(FilePathPolicyFactory, should_return_a_timestamped_file_path_policy_if_configured_to_roll) {
  boost::shared_ptr<FilePathPolicy> policy = FilePathPolicyFactory::createFilePathPolicy("/tmp/", "subdir", true);
  if (!boost::dynamic_pointer_cast<TimestampedFilePathPolicy>(policy)) {
    FAIL();
  }
}

TEST(FilePathPolicyFactory, should_return_a_static_file_path_policy_if_not_configured_to_roll) {
  boost::shared_ptr<FilePathPolicy> policy = FilePathPolicyFactory::createFilePathPolicy("/tmp/", "subdir", false);
  if (!boost::dynamic_pointer_cast<StaticFilePathPolicy>(policy)) {
    FAIL();
  }
}
