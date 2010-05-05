#ifndef BZIP2_INPUT_STREAM_H
#define BZIP2_INPUT_STREAM_H

#include "input_stream.h"
#include <boost/iostreams/filter/bzip2.hpp>

class Bzip2InputStream : public InputStream {
  boost::iostreams::filtering_istream m_stream;
  boost::shared_ptr<InputStream> m_wrapped;
  
public:
  Bzip2InputStream(boost::shared_ptr<InputStream> inputStream)
    : m_wrapped(inputStream) {
    m_stream.push(boost::iostreams::bzip2_decompressor());
    m_stream.push(m_wrapped->inputStream());
  };

  virtual boost::iostreams::filtering_istream & inputStream() {
    return m_stream;
  };
  
  virtual int read(char * outputBuffer, unsigned int numberOfBytesToRead) {
    return boost::iostreams::read(m_stream, outputBuffer, numberOfBytesToRead);
  };
  
  virtual boost::optional<std::string> readEntry() {
    return m_wrapped->readEntry();
  };
};

#endif