if HAVE_GTEST
  AM_CPPFLAGS = -I$(srcdir)/../../src
  AM_CPPFLAGS += $(GTEST_CPPFLAGS) $(BOOST_CPPFLAGS)
  AM_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)
  bin_PROGRAMS = tests 
  tests_SOURCES = dynamic_file_path_policy_test.cpp main.cpp timestamp_helper.cpp file_path_policy_factory_test.cpp static_file_path_policy_test.cpp timestamped_file_path_policy_test.cpp
  tests_LDADD = ../../src/libscribe.a $(GTEST_LIBS)
  tests_CXXFLAGS = -I$(srcdir)/../../src $(GTEST_CXXFLAGS)
  tests_CPPFLAGS = -I$(srcdir)/../../src $(GTEST_CPPFLAGS)
  tests_LDFLAGS = $(GTEST_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)
  
  check:
	./tests
endif