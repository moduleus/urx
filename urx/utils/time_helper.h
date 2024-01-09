#pragma once

#include <algorithm>
#include <ios>
#include <istream>
#include <locale>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace urx::utils {
/**
     * Returns true if the input string is ISO8601
     * Example: "2008-09-15T15:53:00"
     */
inline bool isIso8601(const std::string& dateTime) {
  const std::string r =
      R"(^([\+-]?\d{4}(?!\d{2}\b))((-?)((0[1-9]|1[0-2])(\3([12]\d|0[1-9]|3[01]))?|W([0-4]\d|5[0-2])(-?[1-7])?|(00[1-9]|0[1-9]\d|[12]\d{2}|3([0-5]\d|6[1-6])))([T\s]((([01]\d|2[0-3])((:?)[0-5]\d)?|24\:?00)([\.,]\d+(?!:))?)?(\17[0-5]\d([\.,]\d+)?)?([zZ]|([\+-])([01]\d|2[0-3]):?([0-5]\d)?)?)?)?$)";
  return std::regex_match(dateTime, std::regex(r));
}

/**
     * Returns true if the input string is ISO3166
     * Example: "FR"
     * Note: only checks that it is a 2-letter uppercase
     */
inline bool isIso3166(const std::string& country_code) {
  const std::string r = R"(^[A-Z][A-Z]$)";
  return std::regex_match(country_code, std::regex(r));
}
}  // namespace urx::utils