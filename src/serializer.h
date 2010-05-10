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

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

#define UINT_SIZE 4

class Serializer {
public:
  static std::string serializeUInt(unsigned number) {
    char buffer[UINT_SIZE];
    int i;
    for (i = 0; i < UINT_SIZE; ++i) {
      buffer[i] = (unsigned char)((number >> (8 * i)) & 0xFF);
    }
    return std::string(buffer, UINT_SIZE);
  };
};

#endif