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

#ifndef OUTPUT_STREAM_H
#define OUTPUT_STREAM_H

#include <string>

class OutputStream {
  friend OutputStream & operator<<(OutputStream & out, const std::string & buffer);
  
public:
  virtual ~OutputStream() {};
  virtual void flush() = 0;
  virtual int bytesWritten() const = 0;
  virtual void write(const std::string & buffer) = 0;
};

OutputStream & operator<<(OutputStream & out, const std::string & buffer);

#endif