#include <memory>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <uff_utils/time_helper.h>

namespace uff::test {

TEST_CASE("CHECK_ISO8601", "[DATE ISO]") {
  REQUIRE(isIso8601("2008-09-12T15:53:00"));
  REQUIRE(isIso8601("2019-06-15T15:53:00"));
  REQUIRE(isIso8601("2008-09-15 05:53:59"));
  REQUIRE(isIso8601("2008-09-15 15:53:00.0"));
}

TEST_CASE("CHECK_NOT_ISO8601", "[DATE ISO]") {
  REQUIRE(!isIso8601(""));
  REQUIRE(!isIso8601("2015-01-17T28:23:02Z"));
  REQUIRE(!isIso8601("2009-05-19T14:3924"));
  REQUIRE(!isIso8601("2010-02-18T16:23.33.600"));
  REQUIRE(!isIso8601("2009-05-19T14:3924"));
  REQUIRE(!isIso8601("200-09-15T15:53:00"));
  REQUIRE(!isIso8601("2008-9-15T15:53:00"));
  REQUIRE(!isIso8601("2008-09-5T15:53:00"));
  REQUIRE(!isIso8601("2008-09-15T 5:53:00"));
  REQUIRE(!isIso8601("2008-13-15T15:53:00"));
  REQUIRE(!isIso8601("2008-09-32T15:53:00"));
  REQUIRE(!isIso8601("2008-09-15T25:53:00"));
}

TEST_CASE("CHECK_ISO3166", "[DATE ISO]") {
  REQUIRE(isIso3166("FR"));
  REQUIRE(isIso3166("US"));
  REQUIRE(isIso3166("GE"));
  REQUIRE(isIso3166("IT"));
  REQUIRE(isIso3166("XX"));
}

TEST_CASE("CHECK_NOT_ISO3166", "[DATE ISO]") {
  REQUIRE(!isIso3166("fr"));
  REQUIRE(!isIso3166("FRr"));
  REQUIRE(!isIso3166("France"));
  REQUIRE(!isIso3166(""));
}

}  // namespace uff::test