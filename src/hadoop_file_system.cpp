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

#include "hadoop_file_system.h"

#ifdef USE_SCRIBE_HDFS

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

bool HadoopFileSystem::isSymbolicLink(const std::string & path) const {
  return true;
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

#endif