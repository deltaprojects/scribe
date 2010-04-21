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

#include "dynamic_file_path_policy.h"
#include "hostname.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <boost/tokenizer.hpp>

DynamicFilePathPolicy::DynamicFilePathPolicy(const std::string & aFilePathFormat)
  : filePathFormat(aFilePathFormat) {
  
}

const std::string DynamicFilePathPolicy::directoryPath(struct tm* time, const std::string & category) const {
  return buildFullPath(time, category).branch_path().native_file_string();
}

const std::string DynamicFilePathPolicy::fullPath(struct tm* time, const std::string & category) const {
  return buildFullPath(time, category).native_file_string();
}

const std::string DynamicFilePathPolicy::fileName(struct tm* time, const std::string & category) const {
  return buildFullPath(time, category).leaf();
}

const boost::filesystem::path DynamicFilePathPolicy::buildFullPath(struct tm* time, const std::string & category) const {
  std::string path;
  
  bool isSubstituting = true;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("\%");
  tokenizer tokens(filePathFormat, sep);
  for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    isSubstituting = !isSubstituting;
    if (isSubstituting) {
      path += valueForFormatKey(*it, time, category);
    } else {
      path += *it;
    }
  }
  
  return boost::filesystem::path(path);
}

const std::string DynamicFilePathPolicy::valueForFormatKey(const std::string & aKey, struct tm* time, const std::string & category) const {
  std::ostringstream value;
  
  if (aKey == "year") {
    value << time->tm_year + 1900;
  } else if (aKey == "month") {
    value << std::setw(2) << std::setfill('0') << time->tm_mon + 1;
  } else if (aKey == "day") {
    value << std::setw(2) << std::setfill('0') << time->tm_mday;
  } else if (aKey == "hour") {
    value << std::setw(2) << std::setfill('0') << time->tm_hour;
  } else if (aKey == "minute") {
    value << std::setw(2) << std::setfill('0') << time->tm_min;
  } else if (aKey == "hostname") {
    return Hostname::getHostname();
  } else if (aKey == "category") {
    return category;
  } else {
    std::ostringstream ss;
    ss << "'" << aKey << "' is not a valid path component";
    throw std::runtime_error(ss.str());
  }
  
  return value.str();
}
