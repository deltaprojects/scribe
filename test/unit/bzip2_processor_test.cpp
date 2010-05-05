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
#include <typeinfo>
#include <boost/iostreams/filter/bzip2.hpp>
#include "bzip2_processor.h"
#include <stdexcept>

TEST(Bzip2Processor, should_not_throw_an_exception_if_the_compression_level_is_correct) {
  ASSERT_NO_THROW({
    Bzip2Processor(1);
  });
}

TEST(Bzip2Processor, should_throw_an_exception_if_the_compression_level_is_invalid) {
  ASSERT_THROW({
    Bzip2Processor(0);
  }, std::invalid_argument);
  
  ASSERT_THROW({
    Bzip2Processor(10);
  }, std::invalid_argument);
}