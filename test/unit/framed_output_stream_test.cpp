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
#include "output_stream.h"
#include "framed_output_stream.h"
#include "unserializer.h"

namespace {
  
  class OutputStreamMock : public OutputStream {
  public:
    std::string contents;
    
    virtual void flush() {};
    virtual int bytesWritten() const { return 0; };
    virtual void write(const std::string & buffer) {
      contents += buffer;
    };
    void resetContents() { contents = ""; };
  };
  
}

TEST(FramedOutputStream, should_write_the_length_of_the_message_on_every_write) {
  OutputStreamMock * streamMock = new OutputStreamMock();
  boost::shared_ptr<OutputStream> streamMockPtr(streamMock);
  boost::shared_ptr<OutputStream> framed(new FramedOutputStream(streamMockPtr));
  
  std::vector<std::string> messages;
  messages.push_back("hello");
  messages.push_back("world!");
  
  for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
    framed->write(*it);
    EXPECT_EQ(it->length() + 4, streamMock->contents.length());
    EXPECT_EQ(it->length(), Unserializer::unserializeUInt(streamMock->contents.c_str()));
    streamMock->resetContents();
  }
}
