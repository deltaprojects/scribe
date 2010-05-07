#ifndef BUFFERING_PROCESSOR_H
#define BUFFERING_PROCESSOR_H

#include "processor.h"
#include "buffering_output_stream.h"
#include <iostream>

class BufferingProcessor : public Processor {
  unsigned int m_bufferSize;
  
public:
  BufferingProcessor(unsigned int bufferSize)
    : m_bufferSize(bufferSize) { };
  
  virtual boost::shared_ptr<OutputStream> wrapOutputStream(boost::shared_ptr<OutputStream> outputStream) const {
    return boost::shared_ptr<OutputStream>(new BufferingOutputStream(outputStream, m_bufferSize));
  };
  
  virtual boost::shared_ptr<InputStream> wrapInputStream(boost::shared_ptr<InputStream> inputStream) const {
    return inputStream;
  };
};

#endif