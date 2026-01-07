#include <cstddef>
#include <cstdint>

#include <urx/utils/cpp.h>
#include <urx/utils/io/serialize_helper.h>
#include <urx/utils/serialize_helper.h>

namespace urx::utils::io {

bool use_attribute = false;

const std::unordered_map<std::type_index, const H5::PredType*>& getStdToHdf5() {
  static const std::unordered_map<std::type_index, const H5::PredType*> std_to_hdf5{
      {urx::utils::nameTypeid<float>(), &H5::PredType::NATIVE_FLOAT},
      {urx::utils::nameTypeid<double>(), &H5::PredType::NATIVE_DOUBLE},
      {urx::utils::nameTypeid<std::int8_t>(), &H5::PredType::NATIVE_INT8},
      {urx::utils::nameTypeid<std::uint8_t>(), &H5::PredType::NATIVE_UINT8},
      {urx::utils::nameTypeid<std::int16_t>(), &H5::PredType::NATIVE_INT16},
      {urx::utils::nameTypeid<std::uint16_t>(), &H5::PredType::NATIVE_UINT16},
      {urx::utils::nameTypeid<std::int32_t>(), &H5::PredType::NATIVE_INT32},
      {urx::utils::nameTypeid<std::uint32_t>(), &H5::PredType::NATIVE_UINT32},
      {urx::utils::nameTypeid<std::int64_t>(), &H5::PredType::NATIVE_INT64},
      {urx::utils::nameTypeid<std::uint64_t>(), &H5::PredType::NATIVE_UINT64}};
  return std_to_hdf5;
}

#if defined __has_attribute
#if __has_attribute(no_sanitize)
__attribute__((no_sanitize("undefined")))
#endif
#endif
const std::unordered_map<std::type_index,
                         std::vector<std::pair<urx::utils::AllTypeInVariant, std::string>>>&
getMemberMap() {
  return urx::utils::getMemberMap();
}
}  // namespace urx::utils::io
