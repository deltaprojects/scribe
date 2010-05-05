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
#include "codec_factory.h"
#include "pass_through_codec.h"
#include "bzip2_codec.h"
#include <stdexcept>

TEST(CodecFactory, should_return_a_bzip2_codec) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  boost::shared_ptr<Codec> codec = CodecFactory::createCodec("bzip2", bufferSize, compressionLevel);
  if (!boost::dynamic_pointer_cast<Bzip2Codec>(codec)) {
    FAIL();
  }
}

TEST(CodecFactory, should_return_a_pass_through_codec) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  boost::shared_ptr<Codec> codec = CodecFactory::createCodec("pass", bufferSize, compressionLevel);
  if (!boost::dynamic_pointer_cast<PassThroughCodec>(codec)) {
    FAIL();
  }
}

TEST(CodecFactory, should_throw_an_exception_when_passed_an_invalid_codec_type) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  ASSERT_THROW({
    CodecFactory::createCodec("invalid", bufferSize, compressionLevel);
  }, std::invalid_argument);
}
