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

#ifndef FRAMED_OUTPUT_STREAM_H
#define FRAMED_OUTPUT_STREAM_H

#include "output_stream.h"
#include "serializer.h"

#define UINT_SIZE 4

class FramedOutputStream : public OutputStream {
  boost::shared_ptr<OutputStream> m_wrapped;
  
public:
  FramedOutputStream(boost::shared_ptr<OutputStream> outputStream)
    : m_wrapped(outputStream) { };
  
  ~FramedOutputStream() {
    flush();
  };
  
  virtual void flush() {
    m_wrapped->flush();
  };
  
  virtual int bytesWritten() const {
    return m_wrapped->bytesWritten();
  };

private:
  virtual void write(const std::string & buffer) {
    m_wrapped->write(Serializer::serializeUInt(buffer.length()));
    m_wrapped->write(buffer);
  };
};

#endif