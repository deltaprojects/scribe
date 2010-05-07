#ifndef FRAMED_OUTPUT_STREAM_H
#define FRAMED_OUTPUT_STREAM_H

#include "output_stream.h"

#define UINT_SIZE 4

class FramedOutputStream : public OutputStream {
  boost::shared_ptr<OutputStream> m_wrapped;
  
public:
  FramedOutputStream(boost::shared_ptr<OutputStream> outputStream)
    : m_wrapped(outputStream) { };
  
  ~FramedOutputStream() {
    flush();
  };
  
  virtual void flush() {
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

  virtual void write(const std::string & buffer) {
    m_wrapped->write(serializeUInt(buffer.length()));
    m_wrapped->write(buffer);
  };
};

#endif