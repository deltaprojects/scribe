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

#ifndef FILE_OUTPUT_STREAM_H
#define FILE_OUTPUT_STREAM_H

#include "output_stream.h"
#include <fstream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

class FileOutputStream : public OutputStream {
  int m_fd;
  int m_bytesWritten;
  
public:
  FileOutputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::app | std::ios_base::out)
    : m_bytesWritten(0) {
    int openMode = O_WRONLY | O_CREAT | (mode & std::ios_base::app ? O_APPEND : 0);
    if ((m_fd = open(filename.c_str(), openMode, 0666)) == -1) {
      throw std::runtime_error("failed to open file for writing");
    }
  };
  
  ~FileOutputStream() {
    close(m_fd);
  }
  
  virtual int bytesWritten() const {
    return m_bytesWritten;
  };
  
  virtual void flush() {
    if (fsync(m_fd) != 0) {
      throw std::runtime_error("failed to sync file");
    }
  }

  virtual void write(const std::string & buffer) {
    m_bytesWritten += buffer.length();
    unsigned int written = ::write(m_fd, buffer.c_str(), buffer.length());
    if (written != buffer.length()) {
      throw std::runtime_error("failed to write to file");
    }
  };
};

#endif