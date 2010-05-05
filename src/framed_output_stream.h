#ifndef FRAMED_OUTPUT_STREAM_H
#define FRAMED_OUTPUT_STREAM_H

#include "output_stream.h"

#define UINT_SIZE 4

class FramedOutputStream : public OutputStream {
  boost::iostreams::filtering_ostream m_stream;
  boost::shared_ptr<OutputStream> m_wrapped;
  
public:
  FramedOutputStream(boost::shared_ptr<OutputStream> outputStream)
    : m_wrapped(outputStream) {
    m_stream.push(m_wrapped->outputStream());
  };
  
  ~FramedOutputStream() {
    flush();
    
    m_stream.pop();
  };
  
  virtual boost::iostreams::filtering_ostream & outputStream() {
    return m_stream;
  };
  
  virtual void flush() {
    m_stream.flush();
    m_wrapped->flush();
  };
  
  virtual int bytesWritten() const {
    return m_wrapped->bytesWritten();
  };

private:
  std::string serializeUInt(unsigned number) {
    char buffer[UINT_SIZE];
    int i;
    for (i = 0; i < UINT_SIZE; ++i) {
      buffer[i] = (unsigned char)((number >> (8 * i)) & 0xFF);
    }
    return std::string(buffer, UINT_SIZE);
  };

protected:
  virtual void write(const std::string & buffer) {
    m_stream << serializeUInt(buffer.length());
    m_stream << buffer;
  };
};

#endif