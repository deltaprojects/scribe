#include "output_stream.h"
#include "common.h"
#include <typeinfo>

OutputStream & operator<<(OutputStream & out, const std::string & buffer) {
  out.write(buffer);
  return out;
}
