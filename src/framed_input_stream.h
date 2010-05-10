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

#ifndef FRAMED_INPUT_STREAM_H
#define FRAMED_INPUT_STREAM_H

#include "input_stream.h"
#include "unserializer.h"
#include <vector>

class FramedInputStream : public InputStream {
  boost::iostreams::filtering_istream m_stream;
  boost::shared_ptr<InputStream> m_wrapped;

public:
  FramedInputStream(boost::shared_ptr<InputStream> inputStream)
    : m_wrapped(inputStream) {
    m_stream.push(m_wrapped->inputStream());
  };

  virtual boost::iostreams::filtering_istream & inputStream() {
    return m_stream;
  };
  
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) {
    return boost::iostreams::read(m_stream, outputBuffer, numberOfBytesToRead);
  };
  
  virtual boost::optional<std::string> readEntry() {
    char bytesToReadBuffer[UINT_SIZE];
    
    if (read(bytesToReadBuffer, UINT_SIZE) == UINT_SIZE) {
      int bytesToRead = Unserializer::unserializeUInt(bytesToReadBuffer);
      
      std::vector<char> buffer;
      buffer.reserve(bytesToRead);
      
      if (read(&buffer[0], bytesToRead) == bytesToRead) {
        return boost::optional<std::string>(std::string(&buffer[0], bytesToRead));
      }
    }
    
    return boost::optional<std::string>();
  };
};

#endif