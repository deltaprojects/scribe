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

#ifndef BZIP2_CODEC_H
#define BZIP2_CODEC_H

#include "codec.h"

class Bzip2Codec : public Codec {
public:
  virtual boost::shared_ptr<OutputStream> wrapOutputStream(boost::shared_ptr<OutputStream> outputStream) const;
  virtual boost::shared_ptr<InputStream> wrapInputStream(boost::shared_ptr<InputStream> inputStream) const;
};

#endif