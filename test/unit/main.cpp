#include <gtest/gtest.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace {

  class MinimalPrinter : public EmptyTestEventListener {
   private:
    virtual void OnTestProgramStart(const UnitTest& unit_test) {}
    virtual void OnTestProgramEnd(const UnitTest& unit_test) {}
    virtual void OnTestStart(const TestInfo& test_info) {
      fprintf(stdout, ".");
    }

    virtual void OnTestPartResult(const TestPartResult& test_part_result) {
      fprintf(stdout,
              "%s in %s:%d\n%s\n",
              test_part_result.failed() ? "*** Failure" : "Success",
              test_part_result.file_name(),
              test_part_result.line_number(),
              test_part_result.summary());
      fflush(stdout);
    }
    virtual void OnTestEnd(const TestInfo& test_info) {}
  };

}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
    
  UnitTest& unit_test = *UnitTest::GetInstance();

  TestEventListeners& listeners = unit_test.listeners();
  delete listeners.Release(listeners.default_result_printer());
  listeners.Append(new MinimalPrinter);

  int ret_val = RUN_ALL_TESTS();

  int examples = 0, failures = 0, unexpectedly_failed_tests = 0;
  for (int i = 0; i < unit_test.total_test_case_count(); ++i) {
    const TestCase& test_case = *unit_test.GetTestCase(i);
    for (int j = 0; j < test_case.total_test_count(); ++j) {
      const TestInfo& test_info = *test_case.GetTestInfo(j);
      examples++;
      if (test_info.result()->Failed()) failures++;
    }
  }

  if (unexpectedly_failed_tests == 0)
    ret_val = 0;

  printf("\n%d examples, %d failures\n", examples, failures);
  
  return ret_val;
}
