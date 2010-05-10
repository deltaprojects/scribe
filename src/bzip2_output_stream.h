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

#ifndef BZIP2_OUTPUT_STREAM_H
#define BZIP2_OUTPUT_STREAM_H

#include "output_stream.h"
#include <iostream>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>

class Bzip2OutputStream : public OutputStream {
  boost::iostreams::filtering_ostream m_stream;
  boost::shared_ptr<OutputStream> m_wrapped;
  std::string m_stringOutput;
  
  
public:
  Bzip2OutputStream(boost::shared_ptr<OutputStream> outputStream, int blockSize = boost::iostreams::bzip2::default_block_size)
    : m_wrapped(outputStream) {
    boost::iostreams::bzip2_params params(blockSize);
    
    m_stream.push(boost::iostreams::bzip2_compressor(params));
    m_stream.push(boost::iostreams::back_inserter(m_stringOutput));
  };
  
  virtual ~Bzip2OutputStream() {
    flush();
    
    m_stream.pop();
    m_stream.pop();
    
    flushInternalBuffer();
  };
  
  virtual boost::iostreams::filtering_ostream & outputStream() {
    return m_stream;
  };
  
  void flushInternalBuffer() {
    if (!m_stringOutput.empty()) {
      m_wrapped->write(m_stringOutput);
      m_stringOutput = "";
    }
  };
  
  virtual void flush() {
    m_stream.flush();
    
    flushInternalBuffer();
    
    m_wrapped->flush();
  };
  
  virtual int bytesWritten() const {
    return m_wrapped->bytesWritten();
  };

protected:
  virtual void write(const std::string & buffer) {
    m_stream << buffer;
    flushInternalBuffer();
  };
};

#endif