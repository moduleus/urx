#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace uac {

struct HwConfig {
  template <class... Args>
  struct VariantType {
    using TypeAndVector = std::variant<Args..., std::vector<Args>...>;
  };

  using VecDataTypeVariant =
      VariantType<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float,
                  double, std::string, HwConfig>::TypeAndVector;

  bool operator==(const HwConfig& other) const { return values == other.values; }

  bool operator!=(const HwConfig& other) const { return !operator==(other); }

  std::unordered_map<std::string, VecDataTypeVariant> values;
};

}  // namespace uac