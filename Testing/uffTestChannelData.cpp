/**
 * Copyright MODULEUS 2019
 */

// UFF
#include "uff/channel_data.h"
#include "uff/log.h"

// System
#include <iostream>

#define CHECK_ISO8601(str)                                   \
  if (uff::ChannelData<float>::isIso8601(str) == false) {    \
    LOG_NO_THIS(ERROR) << "Not ISO-8601: " << (str) << "\n"; \
    return -1;                                               \
  }

#define CHECK_NOT_ISO8601(str)                           \
  if (uff::ChannelData<float>::isIso8601(str) == true) { \
    LOG_NO_THIS(ERROR) << "ISO-8601: " << (str) << "\n"; \
    return -1;                                           \
  }

#define CHECK_ISO3166(str)                                   \
  if (uff::ChannelData<float>::isIso3166(str) == false) {    \
    LOG_NO_THIS(ERROR) << "Not ISO-3166: " << (str) << "\n"; \
    return -1;                                               \
  }

#define CHECK_NOT_ISO3166(str)                           \
  if (uff::ChannelData<float>::isIso3166(str) == true) { \
    LOG_NO_THIS(ERROR) << "ISO-3166: " << (str) << "\n"; \
    return -1;                                           \
  }

int main(int argc, char* argv[]) {
  // Test ISO-8601
  CHECK_ISO8601("2008-09-12T15:53:00")
  CHECK_ISO8601("2019-06-15T15:53:00")
  CHECK_ISO8601("2008-09-15 05:53:59")
  CHECK_ISO8601("2008-09-15 15:53:00.0")

  CHECK_NOT_ISO8601("")
  CHECK_NOT_ISO8601("2015-01-17T28:23:02Z")
  CHECK_NOT_ISO8601("2009-05-19T14:3924")
  CHECK_NOT_ISO8601("2010-02-18T16:23.33.600")
  CHECK_NOT_ISO8601("2009-05-19T14:3924")
  CHECK_NOT_ISO8601("200-09-15T15:53:00")
  CHECK_NOT_ISO8601("2008-9-15T15:53:00")
  CHECK_NOT_ISO8601("2008-09-5T15:53:00")
  CHECK_NOT_ISO8601("2008-09-15T 5:53:00")
  CHECK_NOT_ISO8601("2008-13-15T15:53:00")
  CHECK_NOT_ISO8601("2008-09-32T15:53:00")
  CHECK_NOT_ISO8601("2008-09-15T25:53:00")

  // Test ISO-3166
  CHECK_ISO3166("FR")
  CHECK_ISO3166("US")
  CHECK_ISO3166("GE")
  CHECK_ISO3166("IT")
  CHECK_ISO3166("XX")

  CHECK_NOT_ISO3166("fr")
  CHECK_NOT_ISO3166("FRr")
  CHECK_NOT_ISO3166("France")
  CHECK_NOT_ISO3166("")

  return 0;
}
