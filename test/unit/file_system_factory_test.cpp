#include <gtest/gtest.h>
#include "file_system_factory.h"
#include "file_system.h"
#include "standard_file_system.h"
#include "hadoop_file_system.h"

TEST(FileSystemFactory, should_return_a_standard_file_system) {
  boost::shared_ptr<FileSystem> fs = FileSystemFactory::createFileSystem("std");
  if (!boost::dynamic_pointer_cast<StandardFileSystem>(fs)) {
    FAIL();
  }
}

TEST(FileSystemFactory, should_return_an_hdfs_file_system) {
  boost::shared_ptr<FileSystem> fs = FileSystemFactory::createFileSystem("hdfs");
  if (!boost::dynamic_pointer_cast<HadoopFileSystem>(fs)) {
    FAIL();
  }
}

TEST(FileSystemFactory, should_throw_an_exception_for_unknown_type) {
  ASSERT_THROW({
    FileSystemFactory::createFileSystem("invalid");
  }, std::invalid_argument);
}
