#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <stdexcept>

class InputStream;
class OutputStream;

class FileSystem {
public:
  virtual boost::shared_ptr<OutputStream> openForWriting(const std::string & path) const = 0;
  virtual boost::shared_ptr<InputStream> openForReading(const std::string & path) const = 0;
  virtual bool fileExists(const std::string & path) const = 0;
  virtual bool isSymbolicLink(const std::string & path) const = 0;
  virtual void removeFile(const std::string & path) const = 0;
  
  virtual void createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const = 0;
  
  virtual std::vector<std::string> listFilenamesInDirectory(const std::string & path) const = 0;
  virtual void createDirectories(const std::string & path) const = 0;
  virtual void removeDirectory(const std::string & path) const = 0;
  virtual bool directoryExists(const std::string & path) const = 0;
  
  virtual unsigned long fileSize(const std::string & path) const = 0;
};

class FileSystemError : public std::runtime_error {
public:
  FileSystemError(const std::string & msg) : std::runtime_error(msg) {};
};

#endif