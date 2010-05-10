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

#ifndef HADOOP_OUTPUT_STREAM_H
#define HADOOP_OUTPUT_STREAM_H

#ifdef USE_SCRIBE_HDFS

#include "output_stream.h"
#include "hadoop_connection.h"
#include <fstream>
#include <iostream>
#include <errno.h>

class HadoopOutputStream : public OutputStream {
  HadoopConnection m_connection;
  unsigned int m_bytesWritten;
  hdfsFile m_file;
  
public:
  HadoopOutputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::app | std::ios_base::out)
    : m_connection(filename),
      m_bytesWritten(0),
      m_file(0) {
    int hdfsMode = 0;
    if (mode & std::ios_base::out) hdfsMode |= O_WRONLY;
    if (mode & std::ios_base::app) hdfsMode |= O_APPEND;
    m_file = hdfsOpenFile(m_connection.fs(), filename.c_str(), hdfsMode, 0, 0, 0);
    if (m_file == NULL) {
      throw std::runtime_error("failed to open hdfs file for writing");
    }
  };
  
  ~HadoopOutputStream() {
    flush();
    hdfsCloseFile(m_connection.fs(), m_file);
  };
  
  virtual int bytesWritten() const {
    return m_bytesWritten;
  };
  
  virtual void flush() {
    hdfsFlush(m_connection.fs(), m_file);
  };
  
protected:
  virtual void write(const std::string & buffer) {
    unsigned int written = static_cast<unsigned int>(hdfsWrite(m_connection.fs(), m_file, buffer.c_str(), static_cast<tSize>(buffer.length())));
    m_bytesWritten += written;
    if (written != buffer.length()) {
      throw std::runtime_error("failed to write to file");
    }
  };
};

#endif

#endif