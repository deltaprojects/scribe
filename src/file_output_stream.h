#ifndef FILE_OUTPUT_STREAM_H
#define FILE_OUTPUT_STREAM_H

#include "output_stream.h"
#include <fstream>
#include <boost/iostreams/filter/counter.hpp>
#include <boost/iostreams/device/file.hpp>
#include <stdexcept>
#include <fcntl.h>

class FileOutputStream : public OutputStream {
  boost::iostreams::filtering_ostream m_stream;
  boost::iostreams::stream_buffer<boost::iostreams::file_sink> m_sink;
  boost::iostreams::counter m_counter;
  
public:
  FileOutputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::app | std::ios_base::out)
    : m_sink(filename, mode),
      m_counter() {
    
    int fd;
    if ((fd = open(filename.c_str(), O_WRONLY)) == -1) {
      throw std::runtime_error("failed to open file for writing");
    }
    close(fd);
    
    m_stream.push(boost::ref(m_counter));
    m_stream.push(m_sink);
  };
  
  ~FileOutputStream() {
    flush();
    
    m_stream.pop();
    m_stream.pop();
  }
  
  virtual boost::iostreams::filtering_ostream & outputStream() {
    return m_stream;
  };
  
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