#ifndef PASS_THROUGH_CODEC_H
#define PASS_THROUGH_CODEC_H

#include "codec.h"
#include <iostream>

class PassThroughCodec : public Codec {
public:
  virtual boost::shared_ptr<OutputStream> wrapOutputStream(boost::shared_ptr<OutputStream> outputStream) const {
    return outputStream;
  };
  
  virtual boost::shared_ptr<InputStream> wrapInputStream(boost::shared_ptr<InputStream> inputStream) const {
    return inputStream;
  };
};

#endif