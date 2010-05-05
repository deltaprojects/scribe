#ifndef HDFS_DEVICE_HPP
#define HDFS_DEVICE_HPP

#include "hdfs.h"
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/throw_exception.hpp>

namespace io = boost::iostreams;

namespace boost { namespace iostreams {

template<typename Ch>
class basic_hdfs_sink {
public:
  typedef Ch char_type;
  struct category
    : public output,
      public device_tag,
      public closable_tag,
      public flushable_tag
        { };

  basic_hdfs_sink(hdfsFS filesystem, const std::string & url, BOOST_IOS::openmode mode = BOOST_IOS::out)
    : m_fileSystem(filesystem) {
    int hdfsMode = 0;
    if (mode & BOOST_IOS::out) hdfsMode |= O_WRONLY;
    if (mode & BOOST_IOS::app) hdfsMode |= O_APPEND;
    m_file = hdfsOpenFile(filesystem, url.c_str(), hdfsMode, 0, 0, 0);
    if (m_file == NULL) {
      boost::throw_exception(BOOST_IOSTREAMS_FAILURE("failed opening file"));
    }
  };
  
  std::streamsize write(const char_type * s, std::streamsize n) {
    return static_cast<std::streamsize>(hdfsWrite(m_fileSystem, m_file, s, static_cast<tSize>(n)));
  };
  
  void close() {
    if (is_open()) {
      hdfsCloseFile(m_fileSystem, m_file);
    }
  };
  
  bool is_open() const {
    return m_file != NULL;
  };
  
  bool flush() {
    if (is_open()) {
      hdfsFlush(m_fileSystem, m_file);
      return true;
    }
    return false;
  };

private:
  hdfsFS m_fileSystem;
  hdfsFile m_file;
};

typedef basic_hdfs_sink<char> hdfs_sink;

} }

#endif