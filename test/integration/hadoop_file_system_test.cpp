#include <gtest/gtest.h>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include "hadoop_file_system.h"
#include "output_stream.h"
#include "input_stream.h"

#define HDFS_TEST_HOST "hdfs://namenode01.de17a.local:8020"

namespace {
  class HadoopFilesystemTest {
  public:
    // We create a global file system for the tests to minimize the
    // number of connections.
    static HadoopFileSystem & fs() {
      static HadoopFileSystem fs;
      return fs;
    };
  };
}

class HadoopFileSystemWithDirectories : public ::testing::Test {
public:
  HadoopFileSystemWithDirectories()
    : testPath(HDFS_TEST_HOST "/scribe-test/test1") {};
  
  ~HadoopFileSystemWithDirectories() {
    HadoopFilesystemTest::fs().removeDirectory(HDFS_TEST_HOST "/scribe-test");
  };
  
  const std::string testPath;
};

TEST_F(HadoopFileSystemWithDirectories, should_create_directories) {
  ASSERT_FALSE(HadoopFilesystemTest::fs().directoryExists(testPath));
  HadoopFilesystemTest::fs().createDirectories(testPath);
  
  ASSERT_TRUE(HadoopFilesystemTest::fs().directoryExists(testPath));
}

TEST_F(HadoopFileSystemWithDirectories, should_remove_directories) {
  HadoopFilesystemTest::fs().createDirectories(testPath);
  ASSERT_TRUE(HadoopFilesystemTest::fs().directoryExists(testPath));
  
  HadoopFilesystemTest::fs().removeDirectory(testPath);
  ASSERT_FALSE(HadoopFilesystemTest::fs().directoryExists(testPath));
}

TEST_F(HadoopFileSystemWithDirectories, should_see_that_directories_exist) {
  HadoopFilesystemTest::fs().createDirectories(testPath);
  ASSERT_TRUE(HadoopFilesystemTest::fs().directoryExists(testPath));
}

class HadoopFileSystemWithMultipleFiles : public ::testing::Test {
public:
  HadoopFileSystemWithMultipleFiles()
    : fs(),
      filePaths() {
    filePaths.push_back(HDFS_TEST_HOST "/scribe-test/one");
    filePaths.push_back(HDFS_TEST_HOST "/scribe-test/two");
    filePaths.push_back(HDFS_TEST_HOST "/scribe-test/three");
    
    std::sort(filePaths.begin(), filePaths.end());
    std::for_each(filePaths.begin(), filePaths.end(), boost::bind(&HadoopFileSystem::createDirectories, boost::ref(fs), _1));
  };
  
  ~HadoopFileSystemWithMultipleFiles() {
    HadoopFilesystemTest::fs().removeDirectory(HDFS_TEST_HOST "/scribe-test");
  };
  
  const std::string basename(const std::string & path) {
    return boost::filesystem::basename(boost::filesystem::path(path));
  }
  
  HadoopFileSystem fs;
  const std::string testPath;
  std::vector<std::string> filePaths;
};

TEST_F(HadoopFileSystemWithMultipleFiles, should_return_a_list_files_in_given_directory) {
  std::vector<std::string> list = HadoopFilesystemTest::fs().listFilenamesInDirectory(HDFS_TEST_HOST "/scribe-test");
  std::vector<std::string> filenames(filePaths.size());
  std::transform(filePaths.begin(), filePaths.end(), filenames.begin(), boost::bind(&HadoopFileSystemWithMultipleFiles::basename, boost::ref(*this), _1));
  
  EXPECT_TRUE(filenames == list);
}

TEST_F(HadoopFileSystemWithDirectories, should_return_an_empty_list_of_files_if_directory_is_empty) {
  HadoopFilesystemTest::fs().createDirectories(HDFS_TEST_HOST "/scribe-test");
  
  std::vector<std::string> list = HadoopFilesystemTest::fs().listFilenamesInDirectory(HDFS_TEST_HOST "/scribe-test");

  EXPECT_TRUE(list.empty());
}

TEST_F(HadoopFileSystemWithDirectories, should_throw_an_exception_if_directory_does_not_exist) {
  ASSERT_FALSE(HadoopFilesystemTest::fs().directoryExists(HDFS_TEST_HOST "/non-existing-directory"));
  
  ASSERT_THROW({
    HadoopFilesystemTest::fs().listFilenamesInDirectory(HDFS_TEST_HOST "/non-existing-directory");
  }, FileSystemError);
}

class HadoopFileSystemWithFiles : public ::testing::Test {
public:
  HadoopFileSystemWithFiles()
    : fs(),
      testPath(HDFS_TEST_HOST "/scribe-test-out") {
    if (HadoopFilesystemTest::fs().fileExists(testPath)) {
      HadoopFilesystemTest::fs().removeFile(testPath);
    }
  };
  
  ~HadoopFileSystemWithFiles() {
    if (HadoopFilesystemTest::fs().fileExists(testPath)) {
      HadoopFilesystemTest::fs().removeFile(testPath);
    }
  };
  
  void writeStringToTestFile(const std::string & contents) {
    boost::shared_ptr<OutputStream> out = HadoopFilesystemTest::fs().openForWriting(testPath);
    *out << contents;
  };
  
  HadoopFileSystem fs;
  const std::string testPath;
};

TEST_F(HadoopFileSystemWithFiles, should_return_an_output_stream_for_writing_to_disk) {
  const std::string contents = "output testing";
  
  writeStringToTestFile(contents);
  
  EXPECT_EQ(contents.length(), HadoopFilesystemTest::fs().fileSize(testPath));
}

TEST_F(HadoopFileSystemWithFiles, should_remove_files) {
  writeStringToTestFile("test");
  
  ASSERT_TRUE(HadoopFilesystemTest::fs().fileExists(testPath));
  
  HadoopFilesystemTest::fs().removeFile(testPath);
  
  EXPECT_FALSE(HadoopFilesystemTest::fs().fileExists(testPath));
}

TEST_F(HadoopFileSystemWithFiles, should_throw_an_exception_when_removing_a_file_if_it_does_not_exist) {
  ASSERT_THROW({
    HadoopFilesystemTest::fs().removeFile(testPath);
  }, FileSystemError);
}

TEST_F(HadoopFileSystemWithFiles, should_write_the_path_to_the_target_file_to_the_symlink_file_when_creating_symlinks) {
  writeStringToTestFile("test");
  
  const std::string symlinkPath = "/tmp/scribe-test-symlink";
  fs.createSymlink(testPath, symlinkPath);
  
  // We only check to see if the symlink file has any data since we don't support reading.
  EXPECT_TRUE(fs.fileSize(symlinkPath) > 0);
  
  try {
    fs.removeFile(symlinkPath);
  } catch (...) {}
}

TEST_F(HadoopFileSystemWithFiles, should_return_the_correct_filesize) {
  std::string contents = "testing output";
  writeStringToTestFile(contents);
  
  EXPECT_EQ(contents.length(), fs.fileSize(testPath));
}

TEST_F(HadoopFileSystemWithFiles, should_throw_an_exception_if_trying_to_get_an_input_stream) {
  ASSERT_THROW({
    fs.openForReading(testPath);
  }, FileSystemError);
}
