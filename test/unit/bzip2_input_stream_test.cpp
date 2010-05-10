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

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include "input_stream.h"
#include "bzip2_input_stream.h"

namespace {
  
  const char helloWorld[] = {
    0x42, 0x5A, 0x68, 0x39, 0x31, 0x41, 0x59, 0x26,
    0x53, 0x59, 0xBF, 0x4E, 0xBC, 0x25, 0x00, 0x00,
    0x01, 0x17, 0x80, 0x40, 0x04, 0x00, 0x40, 0x00,
    0x80, 0x06, 0x04, 0x90, 0x00, 0x20, 0x00, 0x22,
    0x06, 0x26, 0x42, 0x0C, 0x98, 0x8C, 0x68, 0x35,
    0x20, 0xA8, 0xFC, 0x5D, 0xC9, 0x14, 0xE1, 0x42,
    0x42, 0xFD, 0x3A, 0xF0, 0x94
  };
    
  class InputStreamMock : public InputStream {
    boost::iostreams::filtering_istream m_stream;
    boost::iostreams::stream_buffer<boost::iostreams::basic_array_source<char> > m_source;
    
  public:
    InputStreamMock()
      : m_source(helloWorld, 53) {
      m_stream.push(m_source);
    };
    
    ~InputStreamMock() {
      m_stream.pop();
    }
    
    virtual boost::iostreams::filtering_istream & inputStream() { return m_stream; };
    virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) { return 0; };
    virtual boost::optional<std::string> readEntry() { return boost::optional<std::string>(); };
  };
  
}

TEST(Bzip2InputStream, should_decompress_bzip2_data) {
  boost::shared_ptr<InputStream> streamMock(new InputStreamMock());
  boost::shared_ptr<InputStream> in(new Bzip2InputStream(streamMock));
  
  char buffer[255];
  int len = in->read(buffer, 255);
  buffer[len] = '\0';
  
  EXPECT_EQ("Hello, World", std::string(buffer));
}
