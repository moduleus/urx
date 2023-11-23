#pragma once

#include <cmath>
#include <limits>

struct DoubleNan {
  double value = std::numeric_limits<double>::quiet_NaN();

  DoubleNan() = default;
  DoubleNan(double v) { value = v; };
  DoubleNan(const DoubleNan&) = default;

  auto operator<=>(const DoubleNan& other) const {
    auto c = value <=> other.value;
    if (c == std::partial_ordering::unordered) return std::strong_ordering::equal;
    return c > 0   ? std::strong_ordering::greater
           : c < 0 ? std::strong_ordering::less
                   : std::strong_ordering::equal;
  }

  bool operator==(const DoubleNan& other) const {
    if (std::isnan(value) && std::isnan(other.value)) {
      return true;
    }
    return value == other.value;
  }

  operator double() const { return value; }

  DoubleNan& operator=(double n) {
    value = n;
    return *this;
  }
  DoubleNan operator+(double n) const { return {value + n}; }
  DoubleNan operator-(double n) const { return {value - n}; }
  DoubleNan operator/(double n) const { return {value / n}; }
  DoubleNan operator*(double n) const { return {value * n}; }
  DoubleNan& operator=(const DoubleNan& n) {
    value = n.value;
    return *this;
  }
  DoubleNan& operator+=(const DoubleNan& n) {
    value += n.value;
    return *this;
  }
  DoubleNan& operator-=(const DoubleNan& n) {
    value -= n.value;
    return *this;
  }
  DoubleNan& operator/=(const DoubleNan& n) {
    value /= n.value;
    return *this;
  }
  DoubleNan& operator*=(const DoubleNan& n) {
    value *= n.value;
    return *this;
  }
  DoubleNan operator+(const DoubleNan& n) const { return {value + n.value}; }
  DoubleNan operator-(const DoubleNan& n) const { return {value - n.value}; }
  DoubleNan operator/(const DoubleNan& n) const { return {value / n.value}; }
  DoubleNan operator*(const DoubleNan& n) const { return {value * n.value}; }
};

inline DoubleNan operator+(double n1, const DoubleNan& n2) { return {n1 + n2.value}; }
inline DoubleNan operator-(double n1, const DoubleNan& n2) { return {n1 - n2.value}; }
inline DoubleNan operator/(double n1, const DoubleNan& n2) { return {n1 / n2.value}; }
inline DoubleNan operator*(double n1, const DoubleNan& n2) { return {n1 * n2.value}; }
