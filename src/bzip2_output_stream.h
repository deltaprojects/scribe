#ifndef BZIP2_OUTPUT_STREAM_H
#define BZIP2_OUTPUT_STREAM_H

#include "output_stream.h"
#include <boost/iostreams/filter/bzip2.hpp>

class Bzip2OutputStream : public OutputStream {
  boost::iostreams::filtering_ostream m_stream;
  boost::shared_ptr<OutputStream> m_wrapped;
  
public:
  Bzip2OutputStream(boost::shared_ptr<OutputStream> outputStream, int blockSize = boost::iostreams::bzip2::default_block_size)
    : m_wrapped(outputStream) {
    boost::iostreams::bzip2_params params(blockSize);
    m_stream.push(boost::iostreams::bzip2_compressor(params));
    m_stream.push(m_wrapped->outputStream());
  };
  
  virtual ~Bzip2OutputStream() {
    flush();
    
    m_stream.pop();
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

protected:
  virtual void write(const std::string & buffer) {
    m_stream << buffer;
  };
};

#endif