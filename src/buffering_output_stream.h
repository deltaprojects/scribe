#ifndef BUFFERING_OUTPUT_STREAM_H
#define BUFFERING_OUTPUT_STREAM_H

#include <string>
#include <iostream>
#include "output_stream.h"

class BufferingOutputStream : public OutputStream {
  boost::shared_ptr<OutputStream> m_wrapped;
  std::string m_buffer;
  unsigned int m_bufferSize;
  
public:
  BufferingOutputStream(boost::shared_ptr<OutputStream> outputStream, unsigned int bufferSize)
      : m_wrapped(outputStream),
        m_bufferSize(bufferSize) { };
  
  virtual ~BufferingOutputStream() {
    flush();
  };
  
  virtual void flush() {
    m_wrapped->write(m_buffer);
    m_buffer = "";
    
    m_wrapped->flush();
  };
  
  virtual int bytesWritten() const {
    return m_wrapped->bytesWritten();
  };

protected:
  virtual void write(const std::string & buffer) {
    if (buffer.length() <= 0) return;
    
    m_buffer += buffer;
    
    while (m_buffer.length() >= m_bufferSize) {
      std::string whatToWrite(m_buffer.begin(), m_buffer.begin() + m_bufferSize);
      
      m_wrapped->write(whatToWrite);
      
      std::string tmp(m_buffer.begin() + m_bufferSize, m_buffer.end());
      m_buffer = tmp;
    }
  };
};

#endif