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

#ifndef HADOOP_FILE_SYSTEM_H
#define HADOOP_FILE_SYSTEM_H

#include "file_system.h"

#ifdef USE_SCRIBE_HDFS

#include "hdfs.h"

class HadoopConnection;

class HadoopFileSystem : public FileSystem {
  mutable boost::shared_ptr<HadoopConnection> m_connection;
  
public:
  virtual boost::shared_ptr<OutputStream> openForWriting(const std::string & path) const;
  virtual boost::shared_ptr<InputStream> openForReading(const std::string & path) const;
  virtual bool fileExists(const std::string & path) const;
  virtual bool isSymbolicLink(const std::string & path) const;
  virtual void removeFile(const std::string & path) const;
  
  virtual void createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const;
  
  virtual std::vector<std::string> listFilenamesInDirectory(const std::string & path) const;
  virtual void createDirectories(const std::string & path) const;
  virtual void removeDirectory(const std::string & path) const;
  virtual bool directoryExists(const std::string & path) const;
  
  virtual unsigned long fileSize(const std::string & path) const;
  
  virtual std::string name() const { return "hdfs"; };
  
private:
  bool isDirectory(const std::string& path) const;
  hdfsFS fs(const std::string & path) const;
};

#else

#include <iostream>

class HadoopFileSystem : public FileSystem {
public:
  HadoopFileSystem() {
    std::cerr << "[hdfs] ERROR: HDFS is not supported. If you want HDFS Support, please recompile scribe with HDFS support." << std::endl;
    exit(1);
  }
  virtual boost::shared_ptr<OutputStream> openForWriting(const std::string & path) const { return boost::shared_ptr<OutputStream>(); };
  virtual boost::shared_ptr<InputStream> openForReading(const std::string & path) const { return boost::shared_ptr<InputStream>(); };
  virtual bool fileExists(const std::string & path) const { return false; };
  virtual bool isSymbolicLink(const std::string & path) const { return false; };
  virtual void removeFile(const std::string & path) const {};
  
  virtual void createSymlink(const std::string & sourcePath, const std::string & symlinkPath) const {};
  
  virtual std::vector<std::string> listFilenamesInDirectory(const std::string & path) const { return std::vector<std::string>(); };
  virtual void createDirectories(const std::string & path) const {};
  virtual void removeDirectory(const std::string & path) const {};
  virtual bool directoryExists(const std::string & path) const { return false; };
  
  virtual unsigned long fileSize(const std::string & path) const { return 0; };
  
  virtual std::string name() const { return "hdfs"; };
};

#endif

#endif