#ifndef HADOOP_CONNECTION_H
#define HADOOP_CONNECTION_H

#include <string>
#include "hdfs.h"

class HadoopConnection {
  hdfsFS m_connection;
  
public:
  HadoopConnection(const std::string & url);
  ~HadoopConnection();
  
  hdfsFS fs() const { return m_connection; };
  
private:
  hdfsFS connectToPath(const char * url) const;
};

#endif