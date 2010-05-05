#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/optional.hpp>

class InputStream {
public:
  virtual ~InputStream() {};
  virtual boost::iostreams::filtering_istream & inputStream() = 0;
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) = 0;
  virtual boost::optional<std::string> readEntry() = 0;
};

#endif