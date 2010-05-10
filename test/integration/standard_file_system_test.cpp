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
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include "standard_file_system.h"
#include "output_stream.h"
#include "input_stream.h"

class StandardFileSystemWithDirectories : public ::testing::Test {
public:
  StandardFileSystemWithDirectories()
    : fs(),
      testPath("/tmp/scribe-test/test1") {};
  
  ~StandardFileSystemWithDirectories() {
    fs.removeDirectory("/tmp/scribe-test");
  };
  
  StandardFileSystem fs;
  const std::string testPath;
};

TEST_F(StandardFileSystemWithDirectories, should_create_all_specified_directories) {
  ASSERT_FALSE(fs.directoryExists(testPath));
  fs.createDirectories(testPath);
  
  ASSERT_TRUE(fs.directoryExists(testPath));
}

TEST_F(StandardFileSystemWithDirectories, should_remove_directories) {
  fs.createDirectories(testPath);
  ASSERT_TRUE(fs.directoryExists(testPath));
  
  fs.removeDirectory(testPath);
  ASSERT_FALSE(fs.directoryExists(testPath));
}

TEST_F(StandardFileSystemWithDirectories, should_see_that_directories_exist) {
  fs.createDirectories(testPath);
  ASSERT_TRUE(fs.directoryExists(testPath));
}

class StandardFileSystemWithMultipleFiles : public ::testing::Test {
public:
  StandardFileSystemWithMultipleFiles()
    : fs(),
      filePaths() {
    filePaths.push_back("/tmp/scribe-test/one");
    filePaths.push_back("/tmp/scribe-test/two");
    filePaths.push_back("/tmp/scribe-test/three");
    
    std::sort(filePaths.begin(), filePaths.end());
    
    std::for_each(filePaths.begin(), filePaths.end(), boost::bind(&StandardFileSystem::createDirectories, boost::ref(fs), _1));
  };
  
  ~StandardFileSystemWithMultipleFiles() {
    fs.removeDirectory("/tmp/scribe-test");
  };
  
  const std::string basename(const std::string & path) {
    return boost::filesystem::basename(boost::filesystem::path(path));
  }
  
  StandardFileSystem fs;
  const std::string testPath;
  std::vector<std::string> filePaths;
};

TEST_F(StandardFileSystemWithMultipleFiles, should_return_a_list_files_in_given_directory) {
  std::vector<std::string> list = fs.listFilenamesInDirectory("/tmp/scribe-test");
  std::vector<std::string> filenames(filePaths.size());
  std::transform(filePaths.begin(), filePaths.end(), filenames.begin(), boost::bind(&StandardFileSystemWithMultipleFiles::basename, boost::ref(*this), _1));

  EXPECT_TRUE(filenames == list);
}

TEST_F(StandardFileSystemWithDirectories, should_return_an_empty_list_of_files_if_directory_is_empty) {
  fs.createDirectories("/tmp/scribe-test");
  
  std::vector<std::string> list = fs.listFilenamesInDirectory("/tmp/scribe-test");

  EXPECT_TRUE(list.empty());
}

TEST_F(StandardFileSystemWithDirectories, should_throw_an_exception_if_directory_does_not_exist) {
  ASSERT_FALSE(fs.directoryExists("/tmp/non-existing-directory"));
  
  ASSERT_THROW({
    fs.listFilenamesInDirectory("/tmp/non-existing-directory");
  }, FileSystemError);
}

class StandardFileSystemWithFiles : public ::testing::Test {
public:
  StandardFileSystemWithFiles()
    : fs(),
      testPath("/tmp/scribe-test-out") {
    if (fs.fileExists(testPath)) {
      fs.removeFile(testPath);
    }
  };
  
  ~StandardFileSystemWithFiles() {
    if (fs.fileExists(testPath)) {
      fs.removeFile(testPath);
    }
  };
  
  void writeStringToTestFile(const std::string & contents) {
    boost::shared_ptr<OutputStream> out = fs.openForWriting(testPath);
    *out << contents;
  };
  
  StandardFileSystem fs;
  const std::string testPath;
};

TEST_F(StandardFileSystemWithFiles, should_return_an_output_stream_for_writing_to_disk) {
  const std::string contents = "output testing";
  
  writeStringToTestFile(contents);
  
  EXPECT_EQ(contents.length(), fs.fileSize(testPath));
}

TEST_F(StandardFileSystemWithFiles, should_remove_files) {
  writeStringToTestFile("test");
  
  ASSERT_TRUE(fs.fileExists(testPath));
  
  fs.removeFile(testPath);
  
  EXPECT_FALSE(fs.fileExists(testPath));
}

TEST_F(StandardFileSystemWithFiles, should_throw_an_exception_when_removing_a_file_if_it_does_not_exist) {
  ASSERT_THROW({
    fs.removeFile(testPath);
  }, FileSystemError);
}

TEST_F(StandardFileSystemWithFiles, should_create_symlinks) {
  writeStringToTestFile("test");
  
  const std::string symlinkPath = "/tmp/scribe-test-symlink";
  fs.createSymlink(testPath, symlinkPath);
  
  boost::shared_ptr<InputStream> in = fs.openForReading(symlinkPath);
  char buffer[12 + 1];
  int bytesRead = in->read(buffer, 12);
  buffer[bytesRead] = '\0';
  
  EXPECT_TRUE(strcmp(buffer, "test") == 0);
  
  fs.removeFile(symlinkPath);
}

TEST_F(StandardFileSystemWithFiles, should_return_the_correct_filesize) {
  std::string contents = "testing output";
  writeStringToTestFile(contents);
  
  EXPECT_EQ(contents.length(), fs.fileSize(testPath));
}

TEST_F(StandardFileSystemWithFiles, should_return_an_input_stream_for_reading_from_disk) {
  std::string contents = "testing output";
  writeStringToTestFile(contents);
  
  boost::shared_ptr<InputStream> in = fs.openForReading(testPath);
  char buffer[256 + 1];
  int bytesRead;
  std::string input;
  while ((bytesRead = in->read(buffer, 256)) != -1) {
    buffer[bytesRead] = '\0';
    input += buffer;
  }
  
  EXPECT_EQ(contents, input);
}

TEST_F(StandardFileSystemWithFiles, should_check_to_see_if_a_file_is_a_symlink) {
  writeStringToTestFile("test");
  
  const std::string symlinkPath = "/tmp/scribe-test-symlink";
  fs.createSymlink(testPath, symlinkPath);
  
  EXPECT_TRUE(fs.isSymbolicLink(symlinkPath));
  EXPECT_FALSE(fs.isSymbolicLink(testPath));
  
  fs.removeFile(symlinkPath);
}