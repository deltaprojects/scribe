#ifndef FILE_OUTPUT_STREAM_H
#define FILE_OUTPUT_STREAM_H

#include "output_stream.h"
#include <fstream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

class FileOutputStream : public OutputStream {
  int m_fd;
  int m_bytesWritten;
  
public:
  FileOutputStream(const std::string & filename, std::ios_base::openmode mode = std::ios_base::app | std::ios_base::out)
    : m_bytesWritten(0) {
    int openMode = O_WRONLY | O_CREAT | (mode & std::ios_base::app ? O_APPEND : 0);
    
    mode_t mask = umask((mode_t)0);
    umask(mask);
    
    if ((m_fd = open(filename.c_str(), openMode, S_IRUSR | S_IWUSR)) == -1) {
      throw std::runtime_error("failed to open file for writing");
    }
  };
  
  ~FileOutputStream() {
    close(m_fd);
  }
  
  virtual int bytesWritten() const {
    return m_bytesWritten;
  };
  
  virtual void flush() {
    fsync(m_fd);
  }

  virtual void write(const std::string & buffer) {
    m_bytesWritten += buffer.length();
    ::write(m_fd, buffer.c_str(), buffer.length());
  };
};

#endif