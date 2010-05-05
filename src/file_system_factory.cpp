#include "file_system_factory.h"
#include "standard_file_system.h"
#include "hadoop_file_system.h"

boost::shared_ptr<FileSystem> FileSystemFactory::createFileSystem(const std::string & filesystemType) {
  if (filesystemType == "std") {
    return boost::shared_ptr<FileSystem>(new StandardFileSystem());
  } else if (filesystemType == "hdfs") {
    return boost::shared_ptr<FileSystem>(new HadoopFileSystem());
  } else {
    throw std::invalid_argument("unknown filesystem type");
  }
}
