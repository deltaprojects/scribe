#include "standard_file_system.h"
#include "file_output_stream.h"
#include "file_input_stream.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <unistd.h>

boost::shared_ptr<OutputStream> StandardFileSystem::openForWriting(const std::string & path) const {
  return boost::shared_ptr<OutputStream>(new FileOutputStream(path));
}

boost::shared_ptr<InputStream> StandardFileSystem::openForReading(const std::string & path) const {
  return boost::shared_ptr<InputStream>(new FileInputStream(path));
}

std::vector<std::string> StandardFileSystem::listFilenamesInDirectory(const std::string & path) const {
  std::vector<std::string> list;
  if (directoryExists(path)) {
    boost::filesystem::directory_iterator end;
    for (boost::filesystem::directory_iterator it(path); it != end; ++it) {
      list.push_back(it->filename());
    }
  } else {
    throw FileSystemError("can't list non-existing directory");
  }
  
  return list;
}

void StandardFileSystem::createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const {
  if (symlink(sourcePath.c_str(), symlinkPath.c_str()) != 0) {
    throw FileSystemError("could not create symlink");
  }
}

void StandardFileSystem::createDirectories(const std::string & path) const {
  boost::filesystem::create_directories(path);
}

void StandardFileSystem::removeDirectory(const std::string & path) const {
  if (directoryExists(path)) {
    boost::filesystem::remove_all(path);
  }
}

bool StandardFileSystem::directoryExists(const std::string & path) const {
  return fileExists(path) && boost::filesystem::is_directory(path);
}

void StandardFileSystem::removeFile(const std::string & path) const {
  if (fileExists(path)) {
    boost::filesystem::remove(path);
  } else {
    throw FileSystemError("can't remove non-existing files");
  }
}

bool StandardFileSystem::fileExists(const std::string & path) const {
  return boost::filesystem::exists(path);
}

unsigned long StandardFileSystem::fileSize(const std::string & path) const {
  try {
    return boost::filesystem::file_size(path.c_str());
  } catch(std::exception const & e) {
    throw FileSystemError("couldn't get size of file");
  }
}
