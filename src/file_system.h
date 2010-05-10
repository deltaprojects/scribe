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
  
  virtual std::string name() const = 0;
};

class FileSystemError : public std::runtime_error {
public:
  FileSystemError(const std::string & msg) : std::runtime_error(msg) {};
};

#endif