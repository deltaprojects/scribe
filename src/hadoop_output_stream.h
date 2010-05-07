#ifndef HADOOP_OUTPUT_STREAM_H
#define HADOOP_OUTPUT_STREAM_H

#include "output_stream.h"
#include <boost/iostreams/filter/counter.hpp>
#include "boost/iostreams/device/hdfs.hpp"
#include "hadoop_connection.h"

class HadoopOutputStream : public OutputStream {
  HadoopConnection m_connection;
  boost::iostreams::filtering_ostream m_stream;
  boost::iostreams::stream_buffer<boost::iostreams::hdfs_sink> m_sink;
  boost::iostreams::counter m_counter;
  
public:
  HadoopOutputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::app | std::ios_base::out)
    : m_connection(filename),
      m_sink(m_connection.fs(), filename, mode),
      m_counter() {
    m_stream.push(boost::ref(m_counter));
    m_stream.push(m_sink);
  };
  
  ~HadoopOutputStream() {
    flush();
    
    m_stream.pop();
    m_stream.pop();
  }
  
  virtual int bytesWritten() const {
    return m_counter.characters();
  };
  
  virtual void flush() {
    m_stream.flush();
  }
  
protected:
  virtual void write(const std::string & buffer) {
    m_stream << buffer;
  };
};

#endif