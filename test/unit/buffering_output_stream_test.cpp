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
#include "buffering_output_stream.h"

namespace {
  
  class OutputStreamMock : public OutputStream {
  public:
    std::string contents;
    
    virtual void flush() {};
    virtual int bytesWritten() const { return 0; };
    virtual void write(const std::string & buffer) {
      contents += buffer;
    };
  };
  
}

struct BufferingOutputStreamFixture : public ::testing::Test {
  BufferingOutputStreamFixture()
    : streamMock(new OutputStreamMock()),
      streamMockPtr(streamMock),
      bufferSize(10),
      buffered(new BufferingOutputStream(streamMockPtr, bufferSize)) {
  };
  
  OutputStreamMock * streamMock;
  boost::shared_ptr<OutputStream> streamMockPtr;
  int bufferSize;
  boost::shared_ptr<OutputStream> buffered;
};

TEST_F(BufferingOutputStreamFixture, should_buffer_written_data) {
  buffered->write("Hello, World!");
  EXPECT_EQ(bufferSize, streamMock->contents.length());

  buffered->write("Hello, Again!");
  EXPECT_EQ(bufferSize * 2, streamMock->contents.length());
}

TEST_F(BufferingOutputStreamFixture, should_flush_data) {
  std::string output = "Hello, World!";
  
  buffered->write(output);
  EXPECT_EQ(bufferSize, streamMock->contents.length());
  
  buffered->flush();
  EXPECT_EQ(output.length(), streamMock->contents.length());
}
