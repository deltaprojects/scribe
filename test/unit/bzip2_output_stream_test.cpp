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
#include "output_stream.h"
#include "bzip2_output_stream.h"

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

TEST(Bzip2OutputStream, should_produce_bzip_output) {
  OutputStreamMock * streamMock = new OutputStreamMock();
  boost::shared_ptr<OutputStream> streamMockPtr(streamMock);
  int compressionLevel = 5;
  
  {
    boost::shared_ptr<OutputStream> bzipped(new Bzip2OutputStream(streamMockPtr, compressionLevel));
    bzipped->write("Hello, World!");
  }
  
  // Just check the header to see that the output is bzip2 and that the
  // compression level is correct.
  std::string header(streamMock->contents.begin(), streamMock->contents.begin() + 2);
  std::string compressionLevelString(streamMock->contents.begin() + 3, streamMock->contents.begin() + 4);

  EXPECT_EQ("BZ", header);
  EXPECT_EQ(compressionLevel, atoi(compressionLevelString.c_str()));
}
