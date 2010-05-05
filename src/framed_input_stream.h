#ifndef FRAMED_INPUT_STREAM_H
#define FRAMED_INPUT_STREAM_H

#include "input_stream.h"
#include <vector>

#define UINT_SIZE 4

class FramedInputStream : public InputStream {
  boost::iostreams::filtering_istream m_stream;
  boost::shared_ptr<InputStream> m_wrapped;

private:
  unsigned unserializeUInt(const char * buffer) {
    unsigned retval = 0;
    int i;
    for (i = 0; i < UINT_SIZE; ++i) {
      retval |= (unsigned char)buffer[i] << (8 * i);
    }
    return retval;
  };
  
public:
  FramedInputStream(boost::shared_ptr<InputStream> inputStream)
    : m_wrapped(inputStream) {
    m_stream.push(m_wrapped->inputStream());
  };

  virtual boost::iostreams::filtering_istream & inputStream() {
    return m_stream;
  };
  
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) {
    return boost::iostreams::read(m_stream, outputBuffer, numberOfBytesToRead);
  };
  
  virtual boost::optional<std::string> readEntry() {
    char bytesToReadBuffer[UINT_SIZE];
    
    if (read(bytesToReadBuffer, UINT_SIZE) == UINT_SIZE) {
      int bytesToRead = unserializeUInt(bytesToReadBuffer);
      
      std::vector<char> buffer;
      buffer.reserve(bytesToRead);
      
      if (read(&buffer[0], bytesToRead) == bytesToRead) {
        return boost::optional<std::string>(std::string(&buffer[0], bytesToRead));
      }
    }
    
    return boost::optional<std::string>();
  };
};

#endif