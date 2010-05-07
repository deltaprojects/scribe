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
#include "processor_factory.h"
#include "buffering_processor.h"
#include "bzip2_processor.h"
#include <stdexcept>

TEST(ProcessorFactory, should_return_a_bzip2_processor) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  boost::shared_ptr<Processor> processor = ProcessorFactory::createProcessor("bzip2", bufferSize, compressionLevel);
  if (!boost::dynamic_pointer_cast<Bzip2Processor>(processor)) {
    FAIL();
  }
}

TEST(ProcessorFactory, should_return_a_buffering_processor) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  boost::shared_ptr<Processor> processor = ProcessorFactory::createProcessor("buffered", bufferSize, compressionLevel);
  if (!boost::dynamic_pointer_cast<BufferingProcessor>(processor)) {
    FAIL();
  }
}

TEST(ProcessorFactory, should_throw_an_exception_when_passed_an_invalid_processor_type) {
  int bufferSize = 10000;
  int compressionLevel = 9;
  ASSERT_THROW({
    ProcessorFactory::createProcessor("invalid", bufferSize, compressionLevel);
  }, std::invalid_argument);
}
