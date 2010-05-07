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

#include "processor_factory.h"
#include "bzip2_processor.h"
#include "buffering_processor.h"
#include <stdexcept>

boost::shared_ptr<Processor> ProcessorFactory::createProcessor(const std::string & processorType, unsigned long bufferSize, unsigned long compressionLevel) {
  if (processorType == "bzip2") {
    return boost::shared_ptr<Processor>(new Bzip2Processor(compressionLevel));
  } else if (processorType == "buffered") {
    return boost::shared_ptr<Processor>(new BufferingProcessor(bufferSize));
  } else {
    throw std::invalid_argument("unknown processor");
  }
}
