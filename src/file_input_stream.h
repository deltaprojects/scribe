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

#ifndef FILE_INPUT_STREAM_H
#define FILE_INPUT_STREAM_H

#include "input_stream.h"
#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <iostream>
#include <boost/iostreams/stream.hpp>
#include <stdexcept>

class FileInputStream : public InputStream {
  boost::iostreams::filtering_istream m_stream;
  boost::iostreams::stream_buffer<boost::iostreams::file_source> m_source;
  
public:
  FileInputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::in)
    : m_source(filename, mode) {
    m_stream.push(m_source);
  };
  
  virtual boost::iostreams::filtering_istream & inputStream() {
    return m_stream;
  };
  
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) {
    return boost::iostreams::read(m_stream, outputBuffer, numberOfBytesToRead);
  };
  
  virtual boost::optional<std::string> readEntry() {
    if (!m_stream.good()) {
      return boost::optional<std::string>();
    } else {
      std::string line;
      std::getline(m_stream, line);
      return boost::optional<std::string>(line);
    }
  };
};

#endif