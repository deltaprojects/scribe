#ifndef FILE_SYSTEM_FACTORY_H
#define FILE_SYSTEM_FACTORY_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "file_system.h"

class FileSystemFactory {
public:
  static boost::shared_ptr<FileSystem> createFileSystem(const std::string & filesystemType);
};

#endif