#ifndef STANDARD_FILE_SYSTEM_H
#define STANDARD_FILE_SYSTEM_H

#include "file_system.h"

class StandardFileSystem : public FileSystem {
public:
  virtual boost::shared_ptr<OutputStream> openForWriting(const std::string & path) const;
  virtual boost::shared_ptr<InputStream> openForReading(const std::string & path) const;
  virtual bool fileExists(const std::string & path) const;
  virtual void removeFile(const std::string & path) const;
  
  virtual void createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const;
  
  virtual std::vector<std::string> listFilenamesInDirectory(const std::string & path) const;
  virtual void createDirectories(const std::string & path) const;
  virtual void removeDirectory(const std::string & path) const;
  virtual bool directoryExists(const std::string & path) const;
  
  virtual unsigned long fileSize(const std::string & path) const;
};

#endif