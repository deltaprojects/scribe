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

#include "static_file_path_policy.h"
#include <boost/filesystem.hpp>

StaticFilePathPolicy::StaticFilePathPolicy(const std::string & aBaseFilePath, const std::string & aSubdirectory)
  : baseFilePath(aBaseFilePath)
  , subdirectory(aSubdirectory) {}

const std::string StaticFilePathPolicy::directoryPath(struct tm* time, const std::string & category) const {
  boost::filesystem::path path(baseFilePath);
  path /= boost::filesystem::path(category);
  path /= boost::filesystem::path(subdirectory);
  return path.native_file_string();
}

const std::string StaticFilePathPolicy::fullPath(struct tm* time, const std::string & category) const {
  boost::filesystem::path path(directoryPath(time, category));
  path /= boost::filesystem::path(fileName(time, category));
  return path.native_file_string();
}

const std::string StaticFilePathPolicy::fileName(struct tm* time, const std::string & category) const {
  return category;
}

bool StaticFilePathPolicy::isTimestamped() const {
  return true;
}
