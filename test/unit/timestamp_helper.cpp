#include "timestamp_helper.h"

struct tm TimestampHelper::createTimestamp(int year, int month, int day, int hour, int minute) {
  struct tm timestamp;
  timestamp.tm_year = year - 1900;
  timestamp.tm_mon = month - 1;
  timestamp.tm_mday = day;
  timestamp.tm_hour = hour;
  timestamp.tm_min = minute;
  return timestamp;
}
