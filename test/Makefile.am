EXTERNAL_LIBS = -lfb303 -lthrift -lthriftnb
EXTERNAL_LIBS += -levent -lpthread
EXTERNAL_LIBS += -lhdfs -ljvm

if HAVE_GTEST
  AM_CPPFLAGS = -I$(srcdir)/../src
  AM_CPPFLAGS += $(GTEST_CPPFLAGS) $(BOOST_CPPFLAGS)
  AM_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)
  bin_PROGRAMS = tests
  tests_SOURCES = main.cpp \
				  integration/hadoop_file_system_test.cpp \
				  integration/standard_file_system_test.cpp \
				  unit/dynamic_file_path_policy_test.cpp \
				  unit/timestamp_helper.cpp \
				  unit/file_path_policy_factory_test.cpp \
				  unit/static_file_path_policy_test.cpp \
				  unit/timestamped_file_path_policy_test.cpp \
				  unit/codec_factory_test.cpp \
				  unit/bzip2_codec_test.cpp \
				  unit/file_system_factory_test.cpp
  
  tests_LDADD = ../src/libscribe.a $(GTEST_LIBS) $(EXTERNAL_LIBS)
  tests_CXXFLAGS = -I$(srcdir)/../src $(GTEST_CXXFLAGS) -I$(hadoop_home)/include
  tests_CPPFLAGS = -I$(srcdir)/../src $(GTEST_CPPFLAGS) -I$(hadoop_home)/include
  tests_LDFLAGS = $(GTEST_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_IOSTREAMS_LIB) -L$(thrift_home)/lib -L$(fb303_home)/lib -L$(hadoop_home)/lib
  
  check:
	time ./tests
endif