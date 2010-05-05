#ifndef OUTPUT_STREAM_H
#define OUTPUT_STREAM_H

#include <string>
#include <boost/iostreams/filtering_stream.hpp>

class OutputStream {
  friend OutputStream & operator<<(OutputStream & out, const std::string & buffer);
  
public:
  virtual ~OutputStream() {};
  virtual boost::iostreams::filtering_ostream & outputStream() = 0;
  virtual void flush() = 0;
  virtual int bytesWritten() const = 0;
  
protected:
  virtual void write(const std::string & buffer) = 0;
};

OutputStream & operator<<(OutputStream & out, const std::string & buffer);

#endif