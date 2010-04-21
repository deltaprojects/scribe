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
