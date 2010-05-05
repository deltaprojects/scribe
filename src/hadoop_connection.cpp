#include "hadoop_connection.h"
#include "common.h"
#include "env_default.h"

HadoopConnection::HadoopConnection(const std::string & url) {
  m_connection = connectToPath(url.c_str());
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
 
  // LOG_OPER("[hdfs] Before hdfsConnectNewInstance(%s, %li)", host, port);
  hdfsFS fs = hdfsConnectNewInstance(host, port);
  // LOG_OPER("[hdfs] After hdfsConnectNewInstance");
  free(host);
  return fs;
}
