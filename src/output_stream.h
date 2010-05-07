#ifndef OUTPUT_STREAM_H
#define OUTPUT_STREAM_H

#include <string>

class OutputStream {
  friend OutputStream & operator<<(OutputStream & out, const std::string & buffer);
  
public:
  virtual ~OutputStream() {};
  virtual void flush() = 0;
  virtual int bytesWritten() const = 0;
  virtual void write(const std::string & buffer) = 0;
};

OutputStream & operator<<(OutputStream & out, const std::string & buffer);

#endif