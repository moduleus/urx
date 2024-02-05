#include <memory>
#include <string>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/time_helper.h>

namespace urx::utils::test {

TEST_CASE("CHECK_ISO8601", "[DATE ISO]") {
  REQUIRE(time_helper::isIso8601("2008-09-12T15:53:00"));
  REQUIRE(time_helper::isIso8601("2019-06-15T15:53:00.123"));
  REQUIRE(time_helper::isIso8601("2008-09-15 05:53:59"));
  REQUIRE(time_helper::isIso8601("2008-09-15 15:53:00.0"));
}

TEST_CASE("CHECK_NOT_ISO8601", "[DATE ISO]") {
  REQUIRE(!time_helper::isIso8601(""));
  REQUIRE(!time_helper::isIso8601("2015-01-17T28:23:02Z"));
  REQUIRE(!time_helper::isIso8601("2009-05-19T14:3924"));
  REQUIRE(!time_helper::isIso8601("2010-02-18T16:23.33.600"));
  REQUIRE(!time_helper::isIso8601("2009-05-19T14:3924"));
  REQUIRE(!time_helper::isIso8601("200-09-15T15:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-9-15T15:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-09-5T15:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-09-15T 5:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-13-15T15:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-09-32T15:53:00"));
  REQUIRE(!time_helper::isIso8601("2008-09-15T25:53:00"));
}

TEST_CASE("CHECK_ISO3166", "[DATE ISO]") {
  REQUIRE(time_helper::isIso3166("FR"));
  REQUIRE(time_helper::isIso3166("US"));
  REQUIRE(time_helper::isIso3166("GE"));
  REQUIRE(time_helper::isIso3166("IT"));
  REQUIRE(time_helper::isIso3166("XX"));
}

TEST_CASE("CHECK_NOT_ISO3166", "[DATE ISO]") {
  REQUIRE(!time_helper::isIso3166("fr"));
  REQUIRE(!time_helper::isIso3166("FRr"));
  REQUIRE(!time_helper::isIso3166("France"));
  REQUIRE(!time_helper::isIso3166(""));
}

}  // namespace urx::utils::test