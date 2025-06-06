#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include <H5Cpp.h>

#include <urx/enums.h>
#include <urx/utils/export.h>
#include <urx/utils/io/enums.h>

namespace urx::utils::io::enums {

namespace {
std::unordered_map<urx::ProbeType, std::string> probe_to_string{
    {urx::ProbeType::LINEAR, "LINEAR"}, {urx::ProbeType::CURVILINEAR, "CURVILINEAR"},
    {urx::ProbeType::RCA, "RCA"},       {urx::ProbeType::MATRIX, "MATRIX"},
    {urx::ProbeType::SPARSE, "SPARSE"}, {urx::ProbeType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::ProbeType> string_to_probe = invertMap(probe_to_string);

std::unordered_map<urx::WaveType, std::string> wave_to_string{
    {urx::WaveType::CONVERGING_WAVE, "CONVERGING_WAVE"},
    {urx::WaveType::DIVERGING_WAVE, "DIVERGING_WAVE"},
    {urx::WaveType::PLANE_WAVE, "PLANE_WAVE"},
    {urx::WaveType::CYLINDRICAL_WAVE, "CYLINDRICAL_WAVE"},
    {urx::WaveType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::WaveType> string_to_wave = invertMap(wave_to_string);

std::unordered_map<urx::SamplingType, std::string> sampling_to_string{
    {urx::SamplingType::RF, "RF"},
    {urx::SamplingType::IQ, "IQ"},
    {urx::SamplingType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::SamplingType> string_to_sampling =
    invertMap(sampling_to_string);

std::unordered_map<urx::DataType, std::string> data_to_string{
    {urx::DataType::INT16, "INT16"},
    {urx::DataType::INT32, "INT32"},
    {urx::DataType::FLOAT, "FLOAT"},
    {urx::DataType::DOUBLE, "DOUBLE"},
    {urx::DataType::UNDEFINED, "UNDEFINED"}};
std::unordered_map<std::string, urx::DataType> string_to_data = invertMap(data_to_string);

}  // namespace

template <>
URX_UTILS_EXPORT urx::ProbeType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_probe);
}
template <>
URX_UTILS_EXPORT urx::WaveType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_wave);
}
template <>
URX_UTILS_EXPORT urx::SamplingType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_sampling);
}
template <>
URX_UTILS_EXPORT urx::DataType stringToEnum(const std::string& enumeration) {
  return stringToEnumImpl(enumeration, string_to_data);
}

template <>
URX_UTILS_EXPORT std::string enumToString(urx::ProbeType enumeration) {
  return enumToStringImpl(enumeration, probe_to_string);
}
template <>
URX_UTILS_EXPORT std::string enumToString(urx::WaveType enumeration) {
  return enumToStringImpl(enumeration, wave_to_string);
}
template <>
URX_UTILS_EXPORT std::string enumToString(urx::SamplingType enumeration) {
  return enumToStringImpl(enumeration, sampling_to_string);
}
template <>
URX_UTILS_EXPORT std::string enumToString(urx::DataType enumeration) {
  return enumToStringImpl(enumeration, data_to_string);
}

URX_UTILS_EXPORT urx::DataType h5PredTypeToDataType(const H5::DataType& pred_type) {
  if (pred_type == H5::PredType::NATIVE_INT16) {
    return urx::DataType::INT16;
  }
  if (pred_type == H5::PredType::NATIVE_INT32) {
    return urx::DataType::INT32;
  }
  if (pred_type == H5::PredType::NATIVE_FLOAT) {
    return urx::DataType::FLOAT;
  }
  if (pred_type == H5::PredType::NATIVE_DOUBLE) {
    return urx::DataType::DOUBLE;
  }

  throw std::runtime_error(__FUNCTION__);
}

URX_UTILS_EXPORT const H5::PredType& dataTypeToH5PredType(urx::DataType data_type) {
  if (data_type == urx::DataType::INT16) {
    return H5::PredType::NATIVE_INT16;
  }
  if (data_type == urx::DataType::INT32) {
    return H5::PredType::NATIVE_INT32;
  }
  if (data_type == urx::DataType::FLOAT) {
    return H5::PredType::NATIVE_FLOAT;
  }
  if (data_type == urx::DataType::DOUBLE) {
    return H5::PredType::NATIVE_DOUBLE;
  }

  throw std::runtime_error(__FUNCTION__);
}

}  // namespace urx::utils::io::enums
