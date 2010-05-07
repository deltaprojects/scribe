#include "output_stream.h"

OutputStream & operator<<(OutputStream & out, const std::string & buffer) {
  out.write(buffer);
  return out;
}
