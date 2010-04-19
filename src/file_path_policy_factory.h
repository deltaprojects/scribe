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

#ifndef FILE_PATH_POLICY_FACTORY_H
#define FILE_PATH_POLICY_FACTORY_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "file_path_policy.h"

class FilePathPolicyFactory {
 public:
  static boost::shared_ptr<FilePathPolicy> createFilePathPolicy(const std::string & filePath,
                                                                const std::string & subDirectory,
                                                                bool isConfiguredToRoll);

 private:
  static bool filePathIsDynamicallyConfigured(const std::string & filePath);
};

#endif
