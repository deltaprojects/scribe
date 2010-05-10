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
#include "file_system_factory.h"
#include "file_system.h"
#include "standard_file_system.h"
#include "hadoop_file_system.h"

TEST(FileSystemFactory, should_return_a_standard_file_system) {
  boost::shared_ptr<FileSystem> fs = FileSystemFactory::createFileSystem("/tmp");
  if (!boost::dynamic_pointer_cast<StandardFileSystem>(fs)) {
    FAIL();
  }
}

#ifdef USE_SCRIBE_HDFS

TEST(FileSystemFactory, should_return_an_hdfs_file_system) {
  boost::shared_ptr<FileSystem> fs = FileSystemFactory::createFileSystem("hdfs://somehost:1010/tmp");
  if (!boost::dynamic_pointer_cast<HadoopFileSystem>(fs)) {
    FAIL();
  }
}

#endif