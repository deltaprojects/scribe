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

#ifndef BZIP2_INPUT_STREAM_H
#define BZIP2_INPUT_STREAM_H

#include "input_stream.h"
#include <boost/iostreams/filter/bzip2.hpp>

class Bzip2InputStream : public InputStream {
  boost::iostreams::filtering_istream m_stream;
  boost::shared_ptr<InputStream> m_wrapped;
  
public:
  Bzip2InputStream(boost::shared_ptr<InputStream> inputStream)
    : m_wrapped(inputStream) {
    m_stream.push(boost::iostreams::bzip2_decompressor());
    m_stream.push(m_wrapped->inputStream());
  };

  virtual boost::iostreams::filtering_istream & inputStream() {
    return m_stream;
  };
  
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) {
    return boost::iostreams::read(m_stream, outputBuffer, numberOfBytesToRead);
  };
  
  virtual boost::optional<std::string> readEntry() {
    return m_wrapped->readEntry();
  };
};

#endif