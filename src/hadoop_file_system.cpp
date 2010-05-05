#include "hadoop_file_system.h"
#include "hadoop_output_stream.h"
#include <iostream>
#include "hdfs.h"
#include "hadoop_connection.h"

hdfsFS HadoopFileSystem::fs(const std::string & path) const {
  if (!m_connection) {
    m_connection.reset(new HadoopConnection(path));
  }
  return m_connection->fs();
}

boost::shared_ptr<OutputStream> HadoopFileSystem::openForWriting(const std::string & path) const {
  return boost::shared_ptr<OutputStream>(new HadoopOutputStream(path, std::ios_base::out));
}

boost::shared_ptr<InputStream> HadoopFileSystem::openForReading(const std::string & path) const {
  throw FileSystemError("hdfs does not support reading");
  return boost::shared_ptr<InputStream>();
}

std::vector<std::string> HadoopFileSystem::listFilenamesInDirectory(const std::string & path) const {
  std::vector<std::string> list;
  
  int value = hdfsExists(fs(path), path.c_str());
  if (value == 0) {
    int numEntries = 0;
    hdfsFileInfo * pHdfsFileInfo = 0;
    pHdfsFileInfo = hdfsListDirectory(fs(path), path.c_str(), &numEntries);
    if (pHdfsFileInfo) {
      for(int i = 0; i < numEntries; i++) {
        char* pathname = pHdfsFileInfo[i].mName;
        char* filename = rindex(pathname, '/');
        if (filename != NULL) {
          list.push_back(filename + 1);
        }
      }
      hdfsFreeFileInfo(pHdfsFileInfo, numEntries);
    }
  } else {
    throw FileSystemError("can't list non-existing directory");
  }
  
  std::sort(list.begin(), list.begin());
  
  return list;
}

void HadoopFileSystem::createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const {
  try {
    boost::shared_ptr<OutputStream> targetStream = openForWriting(symlinkPath);
    *targetStream << sourcePath;
  } catch (std::exception & e) {
    throw FileSystemError("could not create symlink");
  }
}

void HadoopFileSystem::createDirectories(const std::string & path) const {
  hdfsCreateDirectory(fs(path), path.c_str());
}

void HadoopFileSystem::removeDirectory(const std::string & path) const {
  if (directoryExists(path)) {
    removeFile(path);
  }
}

bool HadoopFileSystem::isDirectory(const std::string& path) const {
  
  hdfsFileInfo * info = hdfsGetPathInfo(fs(path), path.c_str());
  bool flag = info->mKind == kObjectKindDirectory;
  hdfsFreeFileInfo(info, 1);
  
  return flag;
}

bool HadoopFileSystem::directoryExists(const std::string & path) const {
  return fileExists(path) && isDirectory(path);
}

void HadoopFileSystem::removeFile(const std::string & path) const {
  if (fileExists(path)) {
    hdfsDelete(fs(path), path.c_str());
  } else {
    throw FileSystemError("can't remove non-existing files");
  }
}

bool HadoopFileSystem::fileExists(const std::string & path) const {
  return hdfsExists(fs(path), path.c_str()) == 0;
}

unsigned long HadoopFileSystem::fileSize(const std::string & path) const {
  try {
    unsigned long size = 0;
    hdfsFileInfo* pFileInfo = hdfsGetPathInfo(fs(path), path.c_str());
    
    if (pFileInfo != NULL) {
      size = pFileInfo->mSize;
      hdfsFreeFileInfo(pFileInfo, 1);
    }
    return size;
  } catch(std::exception const & e) {
    throw FileSystemError("couldn't get size of file");
  }
}
