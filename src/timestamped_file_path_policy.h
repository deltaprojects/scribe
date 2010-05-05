//  Copyright (c) 2007-2010 Facebook
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
// @author Björn Sperber
// @author Wouter de Bie

#ifndef TIMESTAMPED_FILE_PATH_POLICY_H
#define TIMESTAMPED_FILE_PATH_POLICY_H

#include "file_path_policy.h"

class TimestampedFilePathPolicy : public FilePathPolicy {
  const std::string baseFilePath;
  const std::string subdirectory;
  
 public:
   TimestampedFilePathPolicy(const std::string & filePathFormat, const std::string & aSubdirectory);
   
   virtual const std::string directoryPath(struct tm* time, const std::string & category) const;
   virtual const std::string fullPath(struct tm* time, const std::string & category) const;
   virtual const std::string fileName(struct tm* time, const std::string & category) const;
   virtual bool supportsReplay() const;
};

#endif
