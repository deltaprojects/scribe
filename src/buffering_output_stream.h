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

#ifndef BUFFERING_OUTPUT_STREAM_H
#define BUFFERING_OUTPUT_STREAM_H

#include <string>
#include <iostream>
#include "output_stream.h"

class BufferingOutputStream : public OutputStream {
  boost::shared_ptr<OutputStream> m_wrapped;
  std::string m_buffer;
  unsigned int m_bufferSize;
  
public:
  BufferingOutputStream(boost::shared_ptr<OutputStream> outputStream, unsigned int bufferSize)
      : m_wrapped(outputStream),
        m_bufferSize(bufferSize) { };
  
  virtual ~BufferingOutputStream() {
    flush();
  };
  
  virtual void flush() {
    m_wrapped->write(m_buffer);
    m_buffer = "";

    m_wrapped->flush();
  };
  
  virtual int bytesWritten() const {
    return m_wrapped->bytesWritten();
  };

protected:
  virtual void write(const std::string & buffer) {
    if (buffer.length() <= 0) return;
    
    m_buffer += buffer;
    while (m_buffer.length() >= m_bufferSize) {
      std::string whatToWrite(m_buffer.begin(), m_buffer.begin() + m_bufferSize);
      
      m_wrapped->write(whatToWrite);
      
      std::string tmp(m_buffer.begin() + m_bufferSize, m_buffer.end());
      m_buffer = tmp;
    }
  };
};

#endif