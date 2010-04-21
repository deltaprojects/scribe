#ifndef TIMESTAMP_HELPER_H
#define TIMESTAMP_HELPER_H

#include <ctime>

struct TimestampHelper {
  static struct tm createTimestamp(int year, int month, int day, int hour = 0, int minute = 0);
};

#endif
