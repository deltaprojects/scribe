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
#include "framed_input_stream.h"
#include "serializer.h"

namespace {
  
  class InputStreamMock : public InputStream {
    boost::iostreams::filtering_istream m_stream;
    boost::iostreams::stream_buffer<boost::iostreams::basic_array_source<char> > m_source;
    
  public:
    InputStreamMock(const char * input, int length)
      : m_source(input, length) {
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

TEST(FramedInputStream, should_read_framed_entries) {
  std::string messages;
  messages += Serializer::serializeUInt(5);
  messages += "hello";
  messages += Serializer::serializeUInt(6);
  messages += "world!";
  
  boost::shared_ptr<InputStream> streamMock(new InputStreamMock(messages.c_str(), messages.length()));
  boost::shared_ptr<InputStream> in(new FramedInputStream(streamMock));
  
  EXPECT_EQ("hello", *in->readEntry());
  EXPECT_EQ("world!", *in->readEntry());
}
