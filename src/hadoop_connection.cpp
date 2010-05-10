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

#include "hadoop_connection.h"

#ifdef USE_SCRIBE_HDFS

#include <limits>
#include "common.h"
#include "env_default.h"
#include <stdexcept>

HadoopConnection::HadoopConnection(const std::string & url) {
  m_connection = connectToPath(url.c_str());
  if (m_connection == 0) {
    throw std::runtime_error("Failed to connect to hdfs");
  }
}

HadoopConnection::~HadoopConnection() {
  hdfsDisconnect(m_connection);
}

hdfsFS HadoopConnection::connectToPath(const char * uri) const {
  const char proto[] = "hdfs://";
 
  if (strncmp(proto, uri, strlen(proto)) != 0) {
    // uri doesn't start with hdfs:// -> use default:0, which is special
    // to libhdfs.
    return hdfsConnectNewInstance("default", 0);
  }
 
  // Skip the hdfs:// part.
  uri += strlen(proto);
  // Find the next colon.
  const char* colon = strchr(uri, ':');
  // No ':' or ':' is the last character.
  if (!colon || !colon[1]) {
    LOG_OPER("[hdfs] Missing port specification: \"%s\"", uri);
    return NULL;
  }
 
  char* endptr = NULL;
  const long port = strtol(colon + 1, &endptr, 10);
  if (port < 0) {
    LOG_OPER("[hdfs] Invalid port specification (negative): \"%s\"", uri);
    return NULL;
  } else if (port > std::numeric_limits<tPort>::max()) {
    LOG_OPER("[hdfs] Invalid port specification (out of range): \"%s\"", uri);
    return NULL;
  }
 
  char* const host = (char*) malloc(colon - uri + 1);
  memcpy((char*) host, uri, colon - uri);
  host[colon - uri] = '\0';
 
  hdfsFS fs = hdfsConnectNewInstance(host, port);
  free(host);
  return fs;
}

#endif
