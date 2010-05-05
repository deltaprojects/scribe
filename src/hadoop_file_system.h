#ifndef HADOOP_FILE_SYSTEM_H
#define HADOOP_FILE_SYSTEM_H

#include "file_system.h"
#include "hdfs.h"

class HadoopConnection;

class HadoopFileSystem : public FileSystem {
  mutable boost::shared_ptr<HadoopConnection> m_connection;
  
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
  
private:
  bool isDirectory(const std::string& path) const;
  hdfsFS fs(const std::string & path) const;
};

#endif